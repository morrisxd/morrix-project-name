/*
 * Copyright (c) 2008-2009, Enea Software AB
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * Neither the name of Enea Software AB nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <signal.h>
#include <regex.h>

#include <linx.h>
#include <ose_gwp.h>

#include "linxgws.h"
#include "linxgws_msghandler.h"

#define MAX_MATCH 1
#define LINE_SIZE 80

struct packet_handler
{
        OseGW_UL packet_type;
        int  (*packet_parser)( int socket , int length , char *payload, 
			       struct ClientInfo *client_data);
};

static int gateway_daemon_termination_flag;
static int gateway_daemon_shutdown_flag;
static char default_conf_file[] = "/etc/linxgws.conf";

/* For each pattern you add here add one in paterns enum */
char *match_strings[] = {
	"INTERFACE_NAME=",
	"GATEWAY_NAME=",
	"PUBLIC_PORT=",
	"BROADCAST_PORT="
};

enum paterns {
	INTERFACE_NAME=0,
	GATEWAY_NAME,
	PUBLIC_PORT,
	BROADCAST_PORT,
	PATTERNS_NUMBER
};

static int broadcast_process(struct configuration_data *cfg);
static int main_process_launcher(struct configuration_data cfg);
static int open_broadcast_fd(char *interface_name, unsigned short port);
static int open_listening_fd(char *interface_name, unsigned short port);
static int client_handler(int client_skt, struct configuration_data cfg);
static int cfg_parser(char *file_name, struct configuration_data *cfg);
static int process_line(char *line, regex_t *expr_conf,
		        struct configuration_data *cfg);

int recv_data(int skt, void *recv_buffer, int to_read)
{
	int status = 0;
	int read_count = 0;

	do {
		status = recv(skt, (char *)recv_buffer + read_count, to_read, 0);
		if (status == -1)
			return -1;

		read_count += status;
		to_read = to_read - status;
	}
	while (to_read);

	return 0;
}

int process_line(char *line, regex_t *expr_conf, struct configuration_data *cfg)
{
	regmatch_t pmatch[MAX_MATCH + 1];
	int status = 0;
	int offset = 0;
	int i = 0;

	for (i = INTERFACE_NAME; i < PATTERNS_NUMBER; i++) {
		status = regexec(&expr_conf[i],line, MAX_MATCH, pmatch, 0);

		if (status != 0){
			continue;
		}
		offset = pmatch[0].rm_eo;
		switch (i) {
		case INTERFACE_NAME:
			strcpy(cfg->interface_name,line + offset);
			/* Clear the new line character */
			cfg->interface_name[strlen(cfg->interface_name) - 1] =
				'\0';
			return status;
			break;
		case GATEWAY_NAME:
			strcpy(cfg->gateway_name,line + offset);
			/* Clear the newline character */
			cfg->gateway_name[strlen(cfg->gateway_name) - 1] = '\0';
			return status;
			break;
		case PUBLIC_PORT:
			cfg->public_port = atoi(line + offset);
			if (cfg->public_port == 0)
				cfg->public_port = DEFAULT_LISTENING_PORT;
			return status;
			break;
		case BROADCAST_PORT:
			cfg->broadcast_port =  atoi(line + offset);
			if (cfg->broadcast_port == 0)
				cfg->broadcast_port = DEFAULT_BROADCAST_PORT;
			return status;
			break;
		default:
			continue;
		};
	}
	return status;
}


int cfg_parser(char *file_name, struct configuration_data *cfg)
{
	regex_t expr_conf[PATTERNS_NUMBER];
	int i;
	int status = 0;
	char conf_line[LINE_SIZE];
	FILE *conf_file;

	printf("open %s\n", file_name);

	/* Compile the regex array for parsing */
	for (i = 0; i < PATTERNS_NUMBER; i++) {
		status = regcomp(expr_conf + i, match_strings[i], REG_EXTENDED);
		if (status != 0) {
			printf("Comp error code %d\n", status);
			return -1;
		}
	}

	conf_file = fopen(file_name, "r");
	if (conf_file == NULL) {
		printf("Parsing configuration file failed!\n");
		return -1;
	}
	while (!feof(conf_file)) {
		fgets(conf_line , LINE_SIZE , conf_file);
		process_line(conf_line , expr_conf, cfg);
	}
	fclose(conf_file);
	return 0;
}

OseGW_UL gw_server_flags(void)
{
	static const int endian_chk = 1;
	OseGW_UL flags = 0UL;

	if (*(char *)&endian_chk == 1)
		flags |= OseGw_SFL_LittleEndian;

	return flags;
}

struct packet_handler packet_list[] = {
	{OseGW_PLT_GenericErrorReply, OseGW_PLT_GenericErrorReply_cbk},
	{OseGW_PLT_InterfaceRequest, OseGW_PLT_InterfaceRequest_cbk},
	{OseGW_PLT_InterfaceReply, OseGW_PLT_InterfaceReply_cbk},
	{OseGW_PLT_LoginRequest, OseGW_PLT_LoginRequest_cbk},
	{OseGW_PLT_ChallengeResponse, OseGW_PLT_ChallengeResponse_cbk},
	{OseGW_PLT_ChallengeReply, OseGW_PLT_ChallengeReply_cbk},
	{OseGW_PLT_LoginReply, OseGW_PLT_LoginReply_cbk},
	{OseGW_PLT_CreateRequest, OseGW_PLT_CreateRequest_cbk},
	{OseGW_PLT_CreateReply, OseGW_PLT_CreateReply_cbk},
	{OseGW_PLT_DestroyRequest, OseGW_PLT_DestroyRequest_cbk},
	{OseGW_PLT_DestroyReply, OseGW_PLT_DestroyReply_cbk},
	{OseGW_PLT_SendRequest, OseGW_PLT_SendRequest_cbk},
	{OseGW_PLT_SendReply, OseGW_PLT_SendReply_cbk},
	{OseGW_PLT_ReceiveRequest, OseGW_PLT_ReceiveRequest_cbk},
	{OseGW_PLT_ReceiveReply, OseGW_PLT_ReceiveReply_cbk},
	{OseGW_PLT_HuntRequest, OseGW_PLT_HuntRequest_cbk},
	{OseGW_PLT_HuntReply, OseGW_PLT_HuntReply_cbk},
	{OseGW_PLT_AttachRequest, OseGW_PLT_AttachRequest_cbk},
	{OseGW_PLT_AttachReply, OseGW_PLT_AttachReply_cbk},
	{OseGW_PLT_DetachRequest, OseGW_PLT_DetachRequest_cbk},
	{OseGW_PLT_DetachReply, OseGW_PLT_DetachReply_cbk},
	{OseGW_PLT_NameRequest, OseGW_PLT_NameRequest_cbk},
	{OseGW_PLT_NameReply, OseGW_PLT_NameReply_cbk}
};

void sigchld_handler(int s)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
                continue;
}

void sighup_handler(int s)
{
	if (gateway_daemon_shutdown_flag == 0) {
		gateway_daemon_shutdown_flag = 1;
		killpg(0, SIGHUP);
	}
}

void sigterm_handler(int s)
{
	if (gateway_daemon_termination_flag == 0) {
		gateway_daemon_shutdown_flag = 1;
		gateway_daemon_termination_flag = 1;
		killpg(0, SIGHUP);
	}
}

int client_handler(int client_skt, struct configuration_data cfg)
{
	struct OseGW_TransportHdr phdr;
	char *payload_buffer;
	struct ClientInfo cinfo;
	struct sigaction sakill;
	int status = 0;
	int size = 0;

	gateway_daemon_shutdown_flag = 0;

	sakill.sa_handler = sighup_handler;
	sigemptyset(&sakill.sa_mask);
	sakill.sa_flags = SA_NOCLDWAIT;
	if (sigaction(SIGHUP, &sakill, NULL) == -1) {
		syslog(LOG_ERR, "Gateway Client: sigaction() for "
                       "SIGTERM failed with %d", errno);
		return -1;
	}

	size = strlen(cfg.gateway_name) + 1;
	cinfo.gw_name = (char *) malloc(size);
	if (cinfo.gw_name == NULL) {
		syslog(LOG_ERR, "Gateway Client: malloc failed");
		return -1;
	}

	strcpy(cinfo.gw_name, cfg.gateway_name);
	while (gateway_daemon_shutdown_flag != 1) {
	        size = sizeof(struct OseGW_TransportHdr);
		status = recv_data(client_skt, (void *) &phdr, size );
		if (status == -1)
		        continue;
		
		/* Convert to native data */
		phdr.payload_type = ntohl(phdr.payload_type);
		phdr.payload_len = ntohl(phdr.payload_len);

		LOG(LOG_INFO, "Gateway Client: got type=%d len=%d",
                    (int)phdr.payload_type, (int)phdr.payload_len);

		if ((phdr.payload_type < 0) &&
                    (phdr.payload_type > OseGW_PLT_LAST_ENTRY)) {
			syslog(LOG_ERR, "Gateway Client: got bad packet "
			       "type %d (len=%d)", (int)phdr.payload_type,
			       (int)phdr.payload_len);
			status = -1;
			break;
		}

		size = phdr.payload_len;
		payload_buffer = malloc(size);
		if (payload_buffer == NULL) {
                        syslog(LOG_ERR, "Gateway Client: malloc failed");
			status = -1;
			continue;
		}

		status = recv_data(client_skt, payload_buffer, size);
		if (status == -1) {
			free(payload_buffer);
			continue;
		}

		/* Get the packet into the protocol handling table */
		status = packet_list[phdr.payload_type].packet_parser(client_skt,
                                                                      size,
                                                                      payload_buffer,
                                                                      &cinfo);
		free(payload_buffer);
		if (status == -1)
			break;
	}
	close(client_skt);
	free(cinfo.gw_name);
	syslog(LOG_INFO, "Gateway Client: shutdown %s",
               gateway_daemon_shutdown_flag ? "due to SIGHUP" : "due to error");
	return status;
}

int open_broadcast_fd(char *interface_name, unsigned short int port)
{
	int bcast_skt;
	struct sockaddr_in ip_address;
	int broadcast_option = 1;
	int status = 0;
	int size = 0;

	bcast_skt = socket(AF_INET, SOCK_DGRAM, 0);
	if (bcast_skt < 0) {
		syslog(LOG_ERR, "Broadcast Daemon: socket() failed with %d",
		       errno);
		return -1;
	}

	status = setsockopt(bcast_skt, SOL_SOCKET, SO_BROADCAST,
			    &broadcast_option, sizeof(int));
	if (status < 0) {
		syslog(LOG_ERR, "Broadcast Daemon: setsockopt() failed with %d",
		       errno);
		close(bcast_skt);
		return -1;
	}
	status = setsockopt(bcast_skt, SOL_SOCKET, SO_REUSEADDR,
			    &broadcast_option, sizeof(int));
	if (status < 0) {
		syslog(LOG_ERR,"Broadcast Daemon: setsockopt() failed with %d",
		       errno);
		close(bcast_skt);
		return -1;
	}

	ip_address.sin_family = AF_INET;
	ip_address.sin_addr.s_addr = INADDR_ANY;
	ip_address.sin_port = htons(port);
	size = sizeof(struct sockaddr_in);
	status = bind(bcast_skt, (struct sockaddr *) &ip_address, size);
	if (status < 0) {
		syslog(LOG_ERR, "Broadcast Daemon: bind() failed with %d",
		       errno);
		close(bcast_skt);
		return -1;
	}

	syslog(LOG_INFO, "Broadcast Daemon: socket opened on address %s:%d",
	       inet_ntoa(ip_address.sin_addr), ntohs(ip_address.sin_port));
	return bcast_skt;
}

int open_listening_fd(char *interface_name, unsigned short port)
{
	int skt;
	struct sockaddr_in ip_address;
	struct ifreq ifr;
	int skt_option = 1;
	int status = 0;
	int size = 0;

	skt = socket(AF_INET, SOCK_STREAM, 0);
	if (skt < 0) {
		syslog(LOG_ERR, "Gateway Daemon: socket() failed with %d",
		       errno);
		return -1;
	}

	size = sizeof(int);
	status = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &skt_option, size);
	if (status == -1) {
		syslog(LOG_ERR, "Gateway Daemon: setsockopt() failed "
                       "%d", errno);
		close(skt);
		return -1;
	}

	/* Set the interface name (from the configuration file) */
	memset(&ifr, 0, sizeof(struct ifreq));
	size = strlen(interface_name) + 1;
	memcpy(&(ifr.ifr_ifrn.ifrn_name), interface_name, size);
	if (ioctl(skt, SIOCGIFADDR, (caddr_t)&ifr, sizeof(struct ifreq)) == -1) {
		close(skt);
                return -1;
	}
	size = sizeof(struct sockaddr_in);
	memcpy(&ip_address, &(ifr.ifr_ifru.ifru_addr), size);
	ip_address.sin_port = htons(port);
	size = sizeof(struct sockaddr_in);
	if (bind(skt, (struct sockaddr *)&ip_address, size) == -1) {
		syslog(LOG_ERR, "Gateway Daemon: bind() failed "
		       "with %d", errno);
		close(skt);
		return -1;
	}
        
        syslog(LOG_INFO, "Gateway Daemon: socket opened on address %s:%d",
               inet_ntoa(ip_address.sin_addr), ntohs(ip_address.sin_port));
	return skt;
}

int main_process_launcher(struct configuration_data cfg)
{
	pid_t broadcast_process_pid;
	pid_t client_process_pid;
	int status;
	int listening_skt;
	int client_skt;
	int client_cnt;
	unsigned int sockaddrin_size = sizeof(struct sockaddr_in);
	struct sockaddr_in client_address;
	struct sigaction sa;
	struct sigaction sakill;
	struct sigaction saterm;

	/* Arm the shutdown flag */
	gateway_daemon_shutdown_flag = 0;
	gateway_daemon_termination_flag = 0;
	while (gateway_daemon_termination_flag != 1) {
		/* Process the configuration file in case of restart (SIGHUP) */
		if (gateway_daemon_shutdown_flag == 1) {
			gateway_daemon_shutdown_flag = 0;
                        openlog("Gateway Daemon:", LOG_PID | LOG_NDELAY,
                                LOG_USER);
			if (cfg_parser(default_conf_file, &cfg) != 0) {
				printf("Error opening configuration file! "
				       "Exiting.\n");
				exit(EXIT_FAILURE);
			}
			syslog(LOG_INFO, "Interface name ..........: %s\n",
			       cfg.interface_name);
			syslog(LOG_INFO, "Gateway name ............: %s\n",
			       cfg.gateway_name);
			syslog(LOG_INFO, "Public port .............: %d\n",
			       cfg.public_port);
			syslog(LOG_INFO, "Broadcast port ..........: %d\n",
			       cfg.broadcast_port);
		}

		/* Overload the signals here */
		sa.sa_handler = sigchld_handler; /* reap all dead processes*/
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1) {
			syslog(LOG_ERR, "Gateway Daemon: sigaction() for "
                               "SIGCHLD failed with %d", errno);
			exit(EXIT_FAILURE);
		}

		sakill.sa_handler = sighup_handler; /* set the restart flag*/
		sigemptyset(&sakill.sa_mask);
		sakill.sa_flags = SA_NOCLDWAIT;
		if (sigaction(SIGHUP, &sakill, NULL) == -1) {
			syslog(LOG_ERR, "Gateway Daemon: error on "
			       "SIGHUP signal handler");
			exit(EXIT_FAILURE);
		}

		saterm.sa_handler = sigterm_handler;/* set the restart flag*/
		sigemptyset(&saterm.sa_mask);
		saterm.sa_flags = SA_NOCLDWAIT;
		if (sigaction(SIGTERM, &saterm, NULL) == -1) {
			syslog(LOG_ERR, "Gateway Daemon: error on "
			       "SIGTERM signal handler");
			exit(EXIT_FAILURE);
		}

		/*
		 * Open socket here for listening on the public tcp port,
		 * default is 16384
                 */
		listening_skt = open_listening_fd(cfg.interface_name,
						  cfg.public_port);
		if (listening_skt == -1) {
			syslog(LOG_ERR, "Gateway Daemon socket listening "
			       "failed. Shutting down!");
			kill(broadcast_process_pid, SIGTERM);
			gateway_daemon_shutdown_flag = 1;
			gateway_daemon_termination_flag =1;
			continue;
		}

		if (listen(listening_skt, MAX_NUMBER_OF_CLIENTS) == -1) {
			syslog(LOG_ERR, "Gateway Daemon socket listening "
			       "failed. Shutting down!");
			kill(broadcast_process_pid, SIGTERM);
			gateway_daemon_shutdown_flag = 1;
			gateway_daemon_termination_flag=1;
			continue;
		}

		/* Launch the broadcast process */
                switch (broadcast_process_pid = fork()) {
                case -1:
                        perror("fork"); /* something went wrong */
                        exit(EXIT_FAILURE);     /* Linux Gateway daemon exits  */
                case 0:
                        status = broadcast_process(&cfg);
                        if (status == -1) {
                                exit(EXIT_FAILURE);
                        } else {
                                exit(EXIT_SUCCESS);
                        }
			break;
                default:
			break;
                }

                syslog(LOG_INFO, "Gateway Daemon started");
                syslog(LOG_INFO, "PID is %d\n", getpid());
		syslog(LOG_INFO, "Broadcast process PID is %d\n",
		       broadcast_process_pid);
                syslog(LOG_INFO, "Group pid %d", getpgid(getpid()));

		client_cnt = 0;
		
		while (!gateway_daemon_shutdown_flag) {

			client_skt = accept(listening_skt,
					    (struct sockaddr *) &client_address,
					    &sockaddrin_size);
			if (client_skt == -1) {
				if (errno == EINTR) {
					syslog(LOG_ERR, "Gateway Daemon "
					       "got a signal!");
					close(listening_skt);
					continue;
				} else {
					syslog(LOG_ERR,"Gateway Daemon "
					       "socket accept failed. "
					       "Shutting down!");
					gateway_daemon_shutdown_flag = 1;
                                        gateway_daemon_termination_flag = 1;
					kill(getpid(), SIGTERM);
					continue;
				}
			}

			syslog(LOG_INFO, "Incomming client %s \n",
			       inet_ntoa(client_address.sin_addr));

			client_process_pid = fork();
			switch (client_process_pid) {
			case -1:
				syslog(LOG_ERR, "Gateway Daemon client "
				       "handling error!");
				kill(getpid(), SIGTERM);
				exit(EXIT_FAILURE);
                                break;
			case 0:
				/* This is the client handling process */
				closelog();
				syslog(LOG_INFO, "Gateway Client pid: %d",
                                       getpid());
				close(listening_skt);
				status = client_handler(client_skt, cfg);
				if (status == -1){
					exit(EXIT_FAILURE);
				} else {
					exit(EXIT_SUCCESS);
				}
				break;
			default:
				close(client_skt);
				client_cnt++;
                                break;
			}
		}

		if (gateway_daemon_termination_flag == 0)
			syslog(LOG_INFO,"Gateway Daemon restarting...");

		while (client_cnt--) {
			int status = 0;
			wait(&status);
		}
		
	}
	close(listening_skt);
	syslog(LOG_INFO, "Gateway Daemon shutting down...");
	closelog();
	return 0;
}

int broadcast_process(struct configuration_data *cfg)
{
	int bcast_skt;
	struct sockaddr_in server_address;
	struct sockaddr_in bcast_addr;
	union LINX_SIGNAL *sig;
	int bcast_pkt_len;
	char revc_buffer[ETHERNET_PACKET_SIZE];
	struct ifreq ifr;
	struct sigaction sakill;
	int size;
	int status = 0;

	openlog("Broadcast Process:", LOG_PID | LOG_NDELAY, LOG_USER);

	/* Handle the restarts/shutdown signal */
	sakill.sa_handler = sighup_handler;
	sigemptyset(&sakill.sa_mask);
	sakill.sa_flags = SA_NOCLDWAIT;
	if (sigaction(SIGHUP, &sakill, NULL) == -1) {
		syslog(LOG_ERR, "Broadcast process: couldn't change SIGHUP "
                       "signal handler (errno = %d)", errno);
		return -1;
	}

	syslog(LOG_INFO, "Gateway Broadcast Process started");
	syslog(LOG_INFO, "Broadcast process on port: %d\n",
	       cfg->broadcast_port);
	syslog(LOG_INFO, "Broadcast process pid is %d\n", getpid());
	syslog(LOG_INFO, "Broadcast process parent's PID is %d\n", getppid());
	syslog(LOG_INFO, "Gateway name: %s", cfg->gateway_name);

	bcast_skt = open_broadcast_fd(cfg->interface_name, cfg->broadcast_port);
	if ( bcast_skt < 0 )
		return -1;

	/* Set the interface name (specified in the configuration file) */
	memset(&ifr, 0, sizeof(struct ifreq));
	size = strlen(cfg->interface_name) + 1;
	memcpy(&(ifr.ifr_ifrn.ifrn_name), cfg->interface_name, size);

	/* Obtain the IP address from that interface */
	size = sizeof(struct ifreq);
	status = ioctl(bcast_skt, SIOCGIFADDR, (caddr_t) & ifr, size);
	if ( status < 0 )
	        return -1;

	size = sizeof(struct sockaddr_in);
	memcpy(&server_address, &(ifr.ifr_ifru.ifru_addr), size);
	sig = malloc(ETHERNET_PACKET_SIZE);
	if (sig == NULL)
	        return -1;        
	bcast_pkt_len = snprintf((char *) sig, ETHERNET_PACKET_SIZE,
				 "OSEGW! %u\n" "Gateway-addr: tcp://%s:%d/\n"
				 "Gateway-name: %s\n", OseGW_ProtocolVersion,
				 inet_ntoa(server_address.sin_addr),
				 cfg->public_port,
				 cfg->gateway_name);

	memset(&bcast_addr, 0, sizeof(struct sockaddr_in));
	bcast_addr.sin_family = AF_INET;
	bcast_addr.sin_port = htons((short)(cfg->broadcast_port & 0xFFFF));
	bcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

	while (gateway_daemon_shutdown_flag == 0) {
		size = strlen("OSEGW?");
		status = recv_data(bcast_skt, revc_buffer, size);
                if (status == -1) {
		        syslog(LOG_INFO, "Broadcast process received malformed "
			       "packet!");
			continue;
		}

		LOG("recv_buffer '%s'\n", revc_buffer);

		if (strncmp("OSEGW?", revc_buffer, size) == 0) {
			syslog(LOG_INFO, "Broadcast process: received "
			       "discovery packet!");
			size = sizeof(struct sockaddr_in);
			sendto(bcast_skt, sig, bcast_pkt_len, 0,
			       (struct sockaddr *) &bcast_addr,
			       size);
		} else {
			syslog(LOG_INFO," Broadcast process: received "
			       "bad discovery packet!");
		}
	}
        close(bcast_skt);
	free(sig);
	closelog();
	return status;
}

int linxgws_main(int argc, char *argv[])
{
	pid_t main_process_pid;
	pid_t sid;
	struct configuration_data cfg;

	if (argc < 2 && cfg_parser(default_conf_file, &cfg) != 0) {
		printf("Error opening configuration file (%s)! Exiting.\n",
		       default_conf_file);
		exit(1);
	} else if (argc == 2 && cfg_parser(argv[1], &cfg) != 0) {
		printf("Error opening configuration file (%s)! Exiting.\n",
		       argv[1]);
		exit(1);
	}

	printf("Interface name ..........: %s\n", cfg.interface_name);
	printf("Gateway name ............: %s\n", cfg.gateway_name);
	printf("Public port .............: %d\n", cfg.public_port);
	printf("Broadcast port ..........: %d\n", cfg.broadcast_port);
	openlog("Linux Gateway Daemon", LOG_PID | LOG_NDELAY, LOG_USER);

	switch (main_process_pid = fork()) {
	case -1:
		perror("fork");	/* something went wrong - fork or die */
		exit(EXIT_FAILURE);/* Linux Gateway daemon exits stage left */
		break;
	case 0:
		openlog("Linux Gateway Daemon:", LOG_PID | LOG_NDELAY,
			LOG_USER);
		/* Change the file mode mask */
		umask(0);
		/* Become the process group leader */
		sid = setsid();
		if (sid < 0)
			exit(EXIT_FAILURE);

		/* Change the current working directory */
		if ((chdir("/")) < 0)
			exit(EXIT_FAILURE);

		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		main_process_launcher(cfg);
		break;
	default:
		printf("Gateway server pid : %d\n", main_process_pid);
	}

	return 0;
}
