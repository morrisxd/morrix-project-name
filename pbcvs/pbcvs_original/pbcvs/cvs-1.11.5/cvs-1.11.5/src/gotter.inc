#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef int SOCKET;

#include "defs.h"
#define CANNOTCONNECT -2
#define MRD_NONE 0
#define CANNOTSOCKET -1
#define BUFMAX 2048
#define _T(x)	x
typedef struct sockaddr * SADDR;

char m_buffer[BUFMAX];
char g_answer[BUFMAX];

void log(char *msg)
{
	printf("%s\n", msg);
}

int m_svrsock;
int WSAGetLastError ()
{
	return errno;
}

int read_from_mr (SOCKET s, char * pmsg)
{
	int len;
	int index = 0;
	char errbuf[2048];

	memset (m_buffer, 0, BUFMAX);
	index = 0;

	while (1)
	{
		if (0 > (len = recv (m_svrsock, &m_buffer[index], BUFMAX, 0))) 
		{
			sprintf (errbuf, \
			"read error, errorcode is [%d]", WSAGetLastError()); 
			log(_T(errbuf));	// WSAEINVAL 
			return -1;
		}
		index += len;
		m_buffer [index] = '\0';
		if ((NULL == strrchr (m_buffer, '\r')) \
			&& (NULL == strrchr (m_buffer, '\n')))
		{
			continue;
		}
		else
			return MRD_NONE;
	}

	return MRD_NONE;
}

int send_to_mr (SOCKET s, char *pmsg)
{
	int len=0;
	int index = 0;

	index = 0;
	sprintf (m_buffer, "hi tim");
	strcat (m_buffer, "\n\r");
	while (1)
	{
		if (0 > (len = send (m_svrsock, &m_buffer[index], \
				strlen(m_buffer), 0)) )
		{
			log (_T("write error"));
			return -2;
		}
		if(len == (strlen(m_buffer)-index)) break;
		index += len;
	}
	

	return MRD_NONE;
}


int querymr (char * pid)
{
	int sock;
	struct sockaddr_in svraddr;

	if (0 > (m_svrsock = socket (AF_INET, SOCK_STREAM, 0)))
	{
		return CANNOTSOCKET;
	}	
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(9378);
	svraddr.sin_addr.s_addr = inet_addr("192.168.11.208");
	if (0 > connect (m_svrsock, (SADDR)&svraddr, sizeof(svraddr)))
	{
		perror ("cannot connect to remote host");
		return CANNOTCONNECT;
	}
	read_from_mr (0, g_answer);
	printf("i have recieved %s\n", m_buffer);
	send_to_mr (0, pid);
	memset (g_answer, 0, BUFMAX);
	read_from_mr (0, g_answer);
	printf("i have recieved %s\n", m_buffer);
	
	return 0;

}

int main (int argc, char * argv[])
{

	CVSMSG msg;
	int id, pid, *pint;

	if (0==fork())
	{
		chdir ("/");
		setsid ();
	}else {
		exit (0);
	}

	pint = (int *)msg.text;
	id = msgget(TEMPMSGKEY, IPC_CREAT | 0666);
	for (;;)
	{
		sleep(1);
		msgrcv(id, &msg.type, MAXMSGLEN, /*1*/RAWID, 0);
		pid = *pint;
		fprintf(stdout,"gotter[%d]: msg recieved [%d],type=[%d]\n", \
				getpid(),pid, msg.type);
		msg.type = pid;
		*pint = 0;

		querymr ("query dwy");

		if('0' == m_buffer[0]){
			*pint = 0;
		}else{
			*pint = 1;
		}

		msgsnd(id, &msg.type, MAXMSGLEN, 0);
		fprintf(stdout,"gotter send ok\n");
	}

	return 0;
}


