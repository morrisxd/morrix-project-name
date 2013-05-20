/*
 * Copyright (c) 2006-2007, Enea Software AB
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

#include <ctype.h>
#include <errno.h>
#include <linx.h>
#include <linx_ioctl.h>
#include <linx_socket.h>
#include <linx_types.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 0x10000

static void *buffer[(BUFFER_SIZE / sizeof(void *))];

struct spid_list {
	LINX_SPID spid;
	pid_t pid;
};

static int get_terminal_width(void)
{
	struct winsize win = {0,0};
	int terminal_width = 0;
	
	ioctl(fileno(stdout), TIOCGWINSZ, &win);
	if (win.ws_col > 0)
		terminal_width = win.ws_col;
	else
		terminal_width = 80;

	return terminal_width;
}

static int wrapped_ioctl(int d, long request, void *arg)
{
	int ret;

	ret = ioctl(d, request, arg);
	if (ret == -1) {
		fprintf(stderr, "ioctl call failed: %s\n", strerror(errno));
		exit(1);
	}

	return ret;
}

static void error_exit(const char *err_msg)
{
	fprintf(stderr, "%s\n", err_msg);
	exit(1);
}

static void usage(void)
{
	printf("Usage: linxstat [-b] [-h] [-aHqSs] [-t <type>]\n");
	printf("Description: Print LINX information specific socket "
	       "information\n");
	printf("\t-a print verbose pending attach information\n");
	printf("\t-b batch mode, print # of linx end points\n");
	printf("\t-f print receive filters\n");
	printf("\t-H print verbose pending hunt information\n");
	printf("\t-h print this help\n");
	printf("\t-q print verbose receive queue information\n");
	printf("\t-S print per socket statistics\n");
	printf("\t-s print socket individual information only\n");
	printf("\t-t print sockets of specified type only\n");
	printf("\t-T print pending timeouts\n");
	exit(0);
}

static char *sockstatetostr(int sockstate)
{
	switch (sockstate) {
	case LINX_STATE_UNKNOWN:
		return "-";
	case LINX_STATE_RUNNING:
		return "run";
	case LINX_STATE_RECV:
		return "recv";
	case LINX_STATE_POLL:
		return "poll";
	default:
		error_exit("LINX spid in unknown state");
		break;
	}
	return "unknown";
}

static char *socktypetostr(int socktype)
{
	switch (socktype) {
	case LINX_TYPE_UNKNOWN:
		return "unknown";
	case LINX_TYPE_LOCAL:
		return "local";
	case LINX_TYPE_REMOTE:
		return "remote";
	case LINX_TYPE_LINK:
		return "link";
	case LINX_TYPE_ILLEGAL:
		return "illegal";
	case LINX_TYPE_ZOMBIE:
		return "zombie";
	default:
		error_exit("LINX spid of unknown type");
	}
	return "unknown";
}

static char *strtolower(const char *str)
{
	int i;
	/* 8 characters is enough - we'll only use strtolower to convert
	 * socket types. */
	static char s[8] = { 0, };

	for (i = 0; str[i]; i++)
		s[i] = tolower(str[i]);

	return s;
}

static int strtosocktype(const char *str)
{
	char *tmp = strtolower(str);

	if (strcmp("unknown", tmp) == 0) {
		return LINX_TYPE_UNKNOWN;
	} else if (strcmp("local", tmp) == 0) {
		return LINX_TYPE_LOCAL;
	} else if (strcmp("remote", tmp) == 0) {
		return LINX_TYPE_REMOTE;
	} else if (strcmp("link", tmp) == 0) {
		return LINX_TYPE_LINK;
	} else if (strcmp("illegal", tmp) == 0) {
		return LINX_TYPE_ILLEGAL;
	} else if (strcmp("zombie", tmp) == 0) {
		return LINX_TYPE_ZOMBIE;
	} else {
		return LINX_TYPE_UNKNOWN;
	}
}

static int getsocktype(int sd, LINX_SPID spid)
{
	struct linx_info info;

	info.type = LINX_INFO_TYPE;
	info.type_spec = buffer;
	((struct linx_info_type *)buffer)->spid = spid;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	return ((struct linx_info_type *)buffer)->type;
}

static void print_queues(int sd, struct spid_list *spid_entry)
{
	int i;
	int no_of_signals;
	struct linx_info info;
	struct linx_info_recv_queue_2 recv_queue;
	struct linx_info_signal_2 *buf;

	info.type = LINX_INFO_RECV_QUEUE_2;
	info.type_spec = &recv_queue;
	recv_queue.spid = spid_entry->spid;
	recv_queue.buffer_size = BUFFER_SIZE;
	recv_queue.buffer = (char *)buffer;

	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	if (recv_queue.no_of_signals > 0)
		printf("\tSignal queue:\n");

	no_of_signals = recv_queue.no_of_signals >
	    recv_queue.buffer_size / (int)sizeof(struct linx_info_signal_2) ?
	    recv_queue.buffer_size / (int)sizeof(struct linx_info_signal_2) :
		recv_queue.no_of_signals;
	buf = (struct linx_info_signal_2 *)recv_queue.buffer;
	for (i = 0; i < no_of_signals; i++) {
		printf("\t [position:%03d signo:%-10d "
		       "size:%-10d from:0x%08x oob:%s]\n",
		       i,
		       buf[i].signo,
		       buf[i].size,
		       buf[i].from,
		       buf[i].flags & MSG_OOB ? "yes" : "no");
	}
	if (no_of_signals < recv_queue.no_of_signals)
		printf("\t [...]\n");
}

static void print_attach(int sd, struct spid_list *spid_entry, int from_or_to)
{
	int i;
	int no_of_attaches;
	struct linx_info info;
	struct linx_info_pend_attach pend_attach;

	info.type = LINX_INFO_PEND_ATTACH;
	info.type_spec = &pend_attach;
	pend_attach.spid = spid_entry->spid;
	pend_attach.from_or_to = from_or_to;
	pend_attach.buffer_size = BUFFER_SIZE;
	pend_attach.buffer = (struct linx_info_attach *)buffer;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	if (pend_attach.no_of_attaches >
	    (BUFFER_SIZE / (int)sizeof(struct linx_info_attach))) {
		printf("\tAttach info (%s):\n",
		       from_or_to == LINX_ATTACH_FROM ? "from" : "to");
		printf("\t [Failed to retrieve attach info]\n");
	} else {
		no_of_attaches = pend_attach.no_of_attaches >
		    pend_attach.buffer_size /
		    (int)sizeof(struct linx_info_attach) ?
		    pend_attach.buffer_size /
		    (int)sizeof(struct linx_info_attach) :
		    pend_attach.no_of_attaches;
		if (no_of_attaches > 0)
			printf("\tAttach info (%s):\n",
			       from_or_to == LINX_ATTACH_FROM ? "from" : "to");
		for (i = 0; i < no_of_attaches; i++) {
			printf("\t [attref:0x%08x   %s:0x%08x "
			       "signo:%-10d size:%6d]\n",
			       pend_attach.buffer[i].attref,
			       from_or_to == LINX_ATTACH_FROM ? "from" : "to",
			       from_or_to == LINX_ATTACH_FROM ?
			       pend_attach.buffer[i].spid :
			       pend_attach.buffer[i].attach_signal.from,
			       pend_attach.buffer[i].attach_signal.signo,
			       pend_attach.buffer[i].attach_signal.size);
		}
		if (no_of_attaches < pend_attach.no_of_attaches)
			printf("\t [...]\n");
	}
}

static void print_hunt(int sd, struct spid_list *spid_entry)
{
	int i;
	int no_of_hunts;
	struct linx_info info;
	struct linx_info_pend_hunt pend_hunt;

	info.type = LINX_INFO_PEND_HUNT;
	info.type_spec = &pend_hunt;
	pend_hunt.spid = spid_entry->spid;
	pend_hunt.buffer_size = BUFFER_SIZE;
	pend_hunt.buffer = (struct linx_info_hunt *)buffer;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	if ((pend_hunt.strings_offset == 0 && pend_hunt.no_of_hunts > 0)) {
		printf("\tPending hunts:\n");
		printf("\t [Failed to retrieve pending hunt info]\n");
	} else {
		no_of_hunts = pend_hunt.no_of_hunts >
		    pend_hunt.strings_offset /
		    (int)sizeof(struct linx_info_hunt) ?
		    pend_hunt.strings_offset /
		    (int)sizeof(struct linx_info_hunt) : pend_hunt.no_of_hunts;
		if (no_of_hunts > 0) {
			printf("\tPending hunts:\n");

			for (i = 0; i < no_of_hunts; i++) {
				printf("\t [signo:%-10d size:%-6d owner:0x%08x"
				       " hunt_name:%s]\n",
				       pend_hunt.buffer[i].hunt_signal.signo,
				       pend_hunt.buffer[i].hunt_signal.size,
				       pend_hunt.buffer[i].owner,
				       pend_hunt.buffer[i].hunt_name);
			}
		}
		if (no_of_hunts < pend_hunt.no_of_hunts)
			printf("\t [...]\n");
	}
}

static void print_sock_stat(int sd, struct spid_list *spid_entry)
{
	int rv;
	struct linx_info info;
	struct linx_info_stat info_stat;

	info.type = LINX_INFO_STAT;
	info.type_spec = &info_stat;
	info_stat.spid = spid_entry->spid;

	/* Can't use wrapped_ioctl here because ioctl()
	   can return ENOSYS if stat is not compiled. */
	rv = ioctl(sd, LINX_IOCTL_INFO, &info);

	if (rv < 0 && errno == ENOSYS) {
		printf("\tSocket statistics not supported "
		       "by the LINX kernel module.\n");
		return;
	}

	if (rv < 0) {
		fprintf(stderr, "ioctl call failed: %s\n", strerror(errno));
		exit(1);
	}

	printf("\tsent bytes:%llu recv bytes:%llu\n",
	       (long long unsigned int)info_stat.no_sent_bytes,
	       (long long unsigned int)info_stat.no_recv_bytes);

	printf("\tsent signals:%llu recv signals:%llu\n",
	       (long long unsigned int)info_stat.no_sent_signals,
	       (long long unsigned int)info_stat.no_recv_signals);

	printf("\tsent local signals:%llu recv local bytes:%llu\n",
	       (long long unsigned int)info_stat.no_sent_local_bytes,
	       (long long unsigned int)info_stat.no_recv_local_bytes);

	printf("\tsent local signals:%llu recv local signals:%llu\n",
	       (long long unsigned int)info_stat.no_sent_local_signals,
	       (long long unsigned int)info_stat.no_recv_local_signals);

	printf("\tsent remote bytes:%llu recv remote bytes:%llu\n",
	       (long long unsigned int)info_stat.no_sent_remote_bytes,
	       (long long unsigned int)info_stat.no_recv_remote_bytes);

	printf("\tsent remote signals:%llu recv remote signals:%llu\n",
	       (long long unsigned int)info_stat.no_sent_remote_signals,
	       (long long unsigned int)info_stat.no_recv_remote_signals);

	printf("\tqueued bytes:%llu queued signals:%llu\n",
	       (long long unsigned int)info_stat.no_queued_bytes,
	       (long long unsigned int)info_stat.no_queued_signals);
}
static void print_filter(int sd, struct spid_list *spid_entry)
{
	int i;
	int no_of_sigselect;
	struct linx_info info;
	struct linx_info_filters filters;

	struct linx_info_state *state;

	/* First check that the process is in RECV or POLL state.  */
	info.type = LINX_INFO_STATE;
	info.type_spec = buffer;
	state = (struct linx_info_state *)buffer;
	state->spid = spid_entry->spid;

	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}

	if (state->state != LINX_STATE_RECV && state->state != LINX_STATE_POLL)
		return;

	info.type = LINX_INFO_FILTERS;
	info.type_spec = &filters;
	filters.spid = spid_entry->spid;
	filters.from_filter = LINX_ILLEGAL_SPID;
	filters.buffer_size = BUFFER_SIZE;
	filters.buffer = (LINX_SIGSELECT *) buffer;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	printf("\tFilters:\n");
	if (filters.no_of_sigselect >
	    (BUFFER_SIZE / (int)sizeof(struct linx_info_filters))) {
		printf("\t [Failed to retrieve filter info]\n");
	} else {
		no_of_sigselect = filters.no_of_sigselect >
		    filters.buffer_size /
		    (int)sizeof(struct linx_info_filters) ?
		    filters.buffer_size /
		    (int)sizeof(struct linx_info_filters) :
		    filters.no_of_sigselect;
		if (no_of_sigselect < 0)
			printf("\t recv but[ ");
		else if (no_of_sigselect == 0)
			printf("\t recv[any]\n");
		else
			printf("\t recv[ ");
		for (i = 0; i < no_of_sigselect; i++) {
			printf("%x ", filters.buffer[i]);
		}
		printf("]\n");
	}
}

static void print_timeouts(int sd, struct spid_list *spid_entry)
{
	int i;
	int no_of_timeouts;
	struct linx_info info;
	struct linx_info_pend_tmo pend_tmo;

	info.type = LINX_INFO_PEND_TMO;
	info.type_spec = &pend_tmo;
	pend_tmo.spid = spid_entry->spid;
	pend_tmo.buffer_size = BUFFER_SIZE;
	pend_tmo.buffer = (struct linx_info_tmo *)buffer;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	if (pend_tmo.no_of_timeouts >
	    (BUFFER_SIZE / (int)sizeof(struct linx_info_tmo))) {
		printf("\tTimeouts:\n");
		printf("\t [Failed to retrieve timeout info]\n");
	} else {
		no_of_timeouts = pend_tmo.no_of_timeouts >
		    pend_tmo.buffer_size /
		    (int)sizeof(struct linx_info_tmo) ?
		    pend_tmo.buffer_size /
		    (int)sizeof(struct linx_info_tmo) : pend_tmo.no_of_timeouts;
		if (no_of_timeouts > 0)
			printf("\tTimeouts:\n");
		for (i = 0; i < no_of_timeouts; i++) {
			printf("\t [tmoref:%#08x   %u signo:%-10d size:%6d]\n",
			       pend_tmo.buffer[i].tmoref,
			       pend_tmo.buffer[i].tmo,
			       pend_tmo.buffer[i].tmo_signal.signo,
			       pend_tmo.buffer[i].tmo_signal.size);
		}
		if (no_of_timeouts < pend_tmo.no_of_timeouts)
			printf("\t [...]\n");
	}
}

static void print_info(int sd, struct spid_list *spid_entry)
{
	struct linx_info info;
	struct linx_info_name *name;
	struct linx_info_type *type;
	struct linx_info_state *state;
	struct linx_info_recv_queue recv_queue;
	struct linx_info_pend_attach pend_attach;
	struct linx_info_pend_hunt pend_hunt;
	struct linx_info_pend_tmo pend_tmo;
	char *ptr;
#define ATTACH_STR_MAX 11
	char tmp[ATTACH_STR_MAX + 1];
	int i, rv, other_width, name_width, terminal_width, contents_width;
	int saved_type;

	printf("%6d ", spid_entry->pid);
	printf("0x%-8x ", spid_entry->spid);

	info.type = LINX_INFO_TYPE;
	info.type_spec = buffer;
	type = (struct linx_info_type *)buffer;
	type->spid = spid_entry->spid;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}

	saved_type = type->type;
	ptr = socktypetostr(type->type);
	printf("%-8s", ptr);

	info.type = LINX_INFO_STATE;
	info.type_spec = buffer;
	state = (struct linx_info_state *)buffer;
	state->spid = spid_entry->spid;

	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}

	/* FIXME: print filter info for sockets in recv state */
	/* FIXME: phantoms shouldn't be "running", they should be "-" */
	if (saved_type == LINX_TYPE_REMOTE || saved_type == LINX_TYPE_LINK) {
		state->state = LINX_STATE_UNKNOWN;
	}

	ptr = sockstatetostr(state->state);
	printf("%-7s", ptr);

	info.type = LINX_INFO_RECV_QUEUE;
	info.type_spec = &recv_queue;
	recv_queue.spid = spid_entry->spid;
	recv_queue.buffer_size = 0;
	recv_queue.buffer = NULL;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}

	printf("%-6d", recv_queue.no_of_signals);

	info.type = LINX_INFO_PEND_ATTACH;
	info.type_spec = &pend_attach;
	pend_attach.spid = spid_entry->spid;
	pend_attach.from_or_to = LINX_ATTACH_FROM;
	pend_attach.buffer_size = 0;
	pend_attach.buffer = NULL;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}
	ptr = tmp;

	/* max 5 places, sorry */
	if (pend_attach.no_of_attaches > 99999)
		i = sprintf(ptr, "XXXXX/");
	else
		i = sprintf(ptr, "%d/", pend_attach.no_of_attaches);
	ptr += i;

	info.type = LINX_INFO_PEND_ATTACH;
	info.type_spec = &pend_attach;
	pend_attach.spid = spid_entry->spid;
	pend_attach.from_or_to = LINX_ATTACH_TO;
	pend_attach.buffer_size = 0;
	pend_attach.buffer = NULL;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}

	/* max 5 places, sorry */
	if (pend_attach.no_of_attaches > 99999)
		sprintf(ptr, "XXXXX");
	else
		sprintf(ptr, "%d", pend_attach.no_of_attaches);

	printf("%-12s", tmp);

	info.type = LINX_INFO_PEND_HUNT;
	info.type_spec = &pend_hunt;
	pend_hunt.spid = spid_entry->spid;
	pend_hunt.buffer_size = 0;
	pend_hunt.buffer = NULL;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}
	printf("%-5d", pend_hunt.no_of_hunts);

	info.type = LINX_INFO_PEND_TMO;
	info.type_spec = &pend_tmo;
	pend_tmo.spid = spid_entry->spid;
	pend_tmo.buffer_size = 0;
	pend_tmo.buffer = NULL;
	if (ioctl(sd, LINX_IOCTL_INFO, &info) < 0) {
		printf("\n");
		return;
	}
	printf("%-5d", pend_tmo.no_of_timeouts);
/* name (maybe truncated) */
	info.type = LINX_INFO_NAME;
	info.type_spec = buffer;
	name = (struct linx_info_name *)buffer;
	name->spid = spid_entry->spid;
	name->namelen = BUFFER_SIZE - sizeof(struct linx_info_name);
	name->name = (char *)(buffer + sizeof(struct linx_info_name));
	/* no wrapper_ioctl here since name may be unassigned */

	rv = ioctl(sd, LINX_IOCTL_INFO, &info);
	if (rv == 0)
		strcpy(name->name, "not bound");
	else if (rv == -1)
		strcpy(name->name, "unknown");

	name->namelen = strlen(name->name);
/* calculate room for name */
	other_width = 61; /* Other fields take 61 characters */
	contents_width = 65; /* TOC takes 65 characters* */
	terminal_width = get_terminal_width();
	if(terminal_width < contents_width) /* TOC wraps anyway, let it wrap */
		name_width = name->namelen;
	else
		name_width = terminal_width - other_width;

	if (name->namelen > name_width) {
		name->name[name_width - 1] = '+';
		name->name[name_width] = '\0';
	}

	printf("%-18s", name->name);
	printf("\n");
#undef ATTACH_STR_MAX
}

int main(int argc, char **argv)
{
	int sd, i;
	int opt;
	int batch = 0;
	int simple = 0;
	int queues = 0;
	int attach = 0;
	int hunt = 0;
	int filters = 0;
	int timeouts = 0;
	int no_of_spids = 0;
	int stat = 0;
	char *socktype = NULL;
	char *cmd;
	struct spid_list *spids = NULL;

	struct linx_info info;
	struct linx_info_sockets sockets;
	struct linx_info_summary *summary;
	struct linx_huntname *huntname;

	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	/* Check arguments. */
	while ((opt = getopt(argc, argv, "abfHhqsSt:T")) != -1) {
		switch (opt) {
		case 'a':
			attach = 1;
			break;
		case 'b':
			batch = 1;
			break;
		case 'f':
			filters = 1;
			break;
		case 'H':
			hunt = 1;
			break;
		case 'h':
			usage();
		case 'q':
			queues = 1;
			break;
		case 'S':
			stat = 1;
			break;
		case 's':
			simple = 1;
			break;
		case 't':
			socktype = optarg;
			break;
		case 'T':
			timeouts = 1;
			break;
		default:
			usage();
		}
	}

	sd = socket(AF_LINX, SOCK_DGRAM, 0);
	if (sd == -1) {
		fprintf(stderr, "Can't open LINX socket: %s\n",
			strerror(errno));
		exit(1);
	}

	huntname = malloc(sizeof(struct linx_huntname) + strlen(argv[0]) + 1);
	if (huntname == NULL) {
		error_exit("Out of memory.\n");
	}
	huntname->name = ((char *)huntname) + sizeof(struct linx_huntname);
	huntname->namelen = strlen(cmd);
	huntname->spid = LINX_ILLEGAL_SPID;
	strcpy(huntname->name, cmd);
	wrapped_ioctl(sd, LINX_IOCTL_HUNTNAME, huntname);

	free(huntname);

	if (batch) {
		info.type = LINX_INFO_SOCKETS;
		info.type_spec = &sockets;
		sockets.local = LINX_TRUE;
		sockets.remote = LINX_TRUE;
		sockets.link = LINX_TRUE;
		sockets.buffer_size = BUFFER_SIZE;
		sockets.buffer = (LINX_SPID *) buffer;
		wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);
		printf("%d\n", sockets.no_of_sockets);
		exit(0);
	}

	if (!simple) {
		info.type = LINX_INFO_SUMMARY;
		info.type_spec = buffer;

		summary = (struct linx_info_summary *)buffer;
		wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

		printf(" local:%d remote:%d link:%d\n"
		       " pending attach:%d\n"
		       " pending hunt:%d\n"
		       " pending timeout:%d\n"
		       " queued signals:%d\n",
		       summary->no_of_local_sockets,
		       summary->no_of_remote_sockets,
		       summary->no_of_link_sockets,
		       summary->no_of_pend_attach,
		       summary->no_of_pend_hunt,
		       summary->no_of_pend_tmo, summary->no_of_queued_signals);
	}
	info.type = LINX_INFO_SOCKETS;
	info.type_spec = &sockets;
	sockets.local = LINX_TRUE;
	sockets.remote = LINX_TRUE;
	sockets.link = LINX_TRUE;
	sockets.buffer_size = BUFFER_SIZE;
	sockets.buffer = (LINX_SPID *) buffer;
	wrapped_ioctl(sd, LINX_IOCTL_INFO, &info);

	no_of_spids = sockets.no_of_sockets;
	if (sockets.buffer_size < no_of_spids * sizeof(LINX_SPID)) {
		printf("Failed to retrieve information on all sockets\n");
		no_of_spids = sockets.buffer_size / sizeof(LINX_SPID);
	}

	spids = malloc(no_of_spids * sizeof(struct spid_list));
	if (spids == NULL) {
		error_exit("Out of memory.\n");
	}

	for (i = 0; i < no_of_spids; i++) {
		struct linx_info_owner owner;

		spids[i].spid = ((LINX_SPID *) (sockets.buffer))[i];

		info.type = LINX_INFO_OWNER;
		info.type_spec = &owner;
		owner.spid = spids[i].spid;

		if (ioctl(sd, LINX_IOCTL_INFO, &info) == -1) {
			spids[i].pid = 0;
		} else {
			spids[i].pid = owner.owner;
		}
	}

	for (i = 0; i < no_of_spids; i++) {
		int j;
		struct spid_list *this = &spids[i];
		struct spid_list *swap = &spids[i];
		struct spid_list tmp;

		for (j = i + 1; j < no_of_spids; j++) {
			if (spids[j].pid < swap->pid)
				swap = &spids[j];
		}
		if (this != swap) {
			memcpy(&tmp, this, sizeof(struct spid_list));
			memcpy(this, swap, sizeof(struct spid_list));
			memcpy(swap, &tmp, sizeof(struct spid_list));
		}
	}

	if (!simple)
		printf("\n   PID spid\t  type"
		       "    state  queue attach(f/t) hunt tmo  name\n");

	for (i = 0; i < no_of_spids; i++) {
		if (spids[i].pid != 0) {
			if (socktype) {
				int cursktype = getsocktype(sd, spids[i].spid);
				int filtersktype = strtosocktype(socktype);
				if (cursktype != filtersktype) {
					continue;
				}
			}
			print_info(sd, &spids[i]);

			if (queues) {
				print_queues(sd, &spids[i]);
			}
			if (attach) {
				print_attach(sd, &spids[i], LINX_ATTACH_FROM);
				print_attach(sd, &spids[i], LINX_ATTACH_TO);
			}
			if (hunt) {
				print_hunt(sd, &spids[i]);
			}
			if (stat) {
				print_sock_stat(sd, &spids[i]);
			}
			if (filters) {
				print_filter(sd, &spids[i]);
			}
			if (timeouts) {
				print_timeouts(sd, &spids[i]);
			}
		}
	}
	free(spids);
	return 0;
}
