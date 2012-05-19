/* V1.0 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

/*
 * This code recieve request from cvs and send the request
 * to MR system.
 * When recieved reply from MR, it will send back to CVS.
 * The intermedia between CVS and me is unix MSGQUEUE.
 */

#include "defs.h"

static int g_msgid_from_cvs = 0;
static int g_msgid_to_cvs = 0;
static int g_semid = 0;
static int g_shmid = 0;
static int g_pid = 0;

static union {
	int val;
	struct semid_ds *buf;
	ushort *array;
} argument;

int Error (char * msg)
{
 	printf("%s\n", msg);
 	return 0;
}

int getshmkey (void)
{
 	return TEMPSHMKEY;
}

int getsemkey (void)
{
 	return TEMPSEMKEY;
}

int getmsgkey (void)
{
 	/* for simplifying the source code, we use #define in header file */
 	/* ftok () */

        return TEMPMSGKEY;
}

int v_op (int semid)
{
	struct sembuf op[1];

	op[0].sem_num = 0;
	op[0].sem_op = 1;
	op[0].sem_flg = 0;

	return semop(semid, op, 1);
}

int p_op (int semid)
{
	struct sembuf op[1];

 	op[0].sem_num = 0;
 	op[0].sem_op = -1;
	op[0].sem_flg = 0;

 	return semop(semid, op, 1);
}

int perm_check_init (void)
{
        struct msqid_ds msgidds;

	g_pid = getpid();

        if (0 > (g_msgid_from_cvs=msgget (getmsgkey(),IPC_CREAT | 0666)))
        {
                perror ("[msg_from]");
                exit (0);
        }
        printf ("msgid_from_cvs = %d\n", g_msgid_from_cvs);
        // msgctl (g_msgid_from_cvs, IPC_STAT, &msgidds);

/********************************************************************\
        if (0 > (g_semid=semget (getsemkey (), 1, IPC_CREAT | 0666)))
        {
                perror ("[sem]");
                exit (0);
        }

        argument.val = 1;
        if( semctl(g_semid, 0, SETVAL, argument) < 0)
        {
                fprintf ( stderr, "Cannot set semaphore value. \n");
        }
        else
        {
                fprintf (stderr, "Semaphore %d initialized. \n", getsemkey ());
        }

        p_op(g_semid)?printf("P Error\n"):printf("P ok\n");
        v_op(g_semid)?printf("V Error\n"):printf("V ok\n");
\********************************************************************/

        return 0;
}

int run ()
{
 	CVSMSG msg;
        ssize_t num;
	int * pint;

	/* send msg as type=pid */
        memset (&msg, 0, sizeof(CVSMSG));
        sprintf (msg.text, "%s", "helloworld");
        msg.len = strlen (msg.text);
	msg.len = MAXMSGLEN;
        msg.type = RAWID/*1*/;
	pint = (int *)msg.text;
	*pint = g_pid;
        if (0 != msgsnd (g_msgid_from_cvs, &msg.type, msg.len, 0))
        {
                error (0, 0, "msgsnd");
        }

	/* recieve msg as type=pid */
        memset (&msg, 0, sizeof(CVSMSG));
        num = msgrcv (g_msgid_from_cvs, &msg.type,MAXMSGLEN,g_pid,0);
        // msg.text[num] = 0;
        printf("recieve[%s]\n", msg.text);

        return *pint;
}

/*
 * for commit () call to check user perm
 */
int perm_check (void)
{
	perm_check_init ();

	return run ();
}

