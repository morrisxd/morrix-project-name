
extern int isdebug (void);

#include "defs.h"


#define CANNOTCONNECT -2
#define MRD_NONE 0
#define CANNOTSOCKET -1
#define BUFMAX 2048
#define _T(x)	x


#if 0

static char m_qh[] = { "GET http://mrserver/cvsmr/addfilelist.asp?" };
static char m_qhid[] = { "GET http://mrserver/cvsmr/querymrid.asp?" };
static char m_utag[] = { "GET http://mrserver/cvsmr/utag.asp?" };
static char m_qe[] = { "HTTP/1.0\r\n" "accept: www/source; text/html; image/gif; image/jpeg; */*\r\n" "User_Agent: myAgent\r\n\r\n"
};

static char m_qs[2048];

static char m_tag[] = { "GET http://mrserver/cvsmr/addfilelist.asp?id=007&filename=james.c&pathname=/bond&mrid=007&comment=thisisjamesbond HTTP/1.0\r\n"
                "accept: www/source; text/html; image/gif; image/jpeg; */*\r\n" "User_Agent: myAgent\r\n\r\n"
};
static char m_querymrid[] = { "GET http://mrserver/cvsmr/querymrid.asp?mrid=MW8100(R1.18)_290&MYNAME=kyang&FASONG=SEND HTTP/1.0\r\n"
                "accept: www/source; text/html; image/gif; image/jpeg; */*\r\n" "User_Agent: myAgent\r\n\r\n"
};


static char m_buffer[BUFMAX];
static char g_answer[BUFMAX];



int
isdebug (void)
{
        return (getenv ("CVSMRDEBUG")) ? 1 : 0;
}


static void
log (msg)
char *msg;
{
        error (0, 0, msg);
}

static int m_svrsock;
static int
WSAGetLastError ()
{
        return errno;
}

static int
read_from_mr (s, pmsg)
SOCKET s;
char *pmsg;
{
        int len;
        int index = 0;
        char errbuf[2048];
        fd_set fdrs, fdws, fdes;
        struct timeval timeout;


        memset (m_buffer, 0, BUFMAX);
        memset (m_qs, 0, sizeof (2048));
        index = 0;
        while (1)
        {
                FD_ZERO (&fdrs);
                FD_SET (m_svrsock, &fdrs);

                switch (select (m_svrsock + 1, &fdrs, NULL, NULL, NULL))
                {
                case -1:
                case 0:
                        error (1, 0, "read socket error");
                        break;
                default:
                        if (FD_ISSET (m_svrsock, &fdrs))
                        {
                                len = recv (m_svrsock, &m_buffer[index], BUFMAX, 0);
                                if (0 > len)
                                {
                                        sprintf (errbuf, "read error, errorcode is [%d]", WSAGetLastError ());
                                        log (_T (errbuf));      // WSAEINVAL 
                                        /* return -1; */
                                        goto readend;
                                }
                                else if (0 == len)
                                {
                                        goto readend;
                                }
                                index += len;
                                m_buffer[index] = '\0';
                                continue;
                                /*
                                   if ((NULL == strrchr (m_buffer, '\r')) \
                                   && (NULL == strrchr (m_buffer, '\n')))
                                   {
                                   continue;
                                   } else{
                                   return MRD_NONE;
                                   }
                                 */
                                break;
                        }       /* end of if (FD_ISSET()) */
                        else
                        {
                                goto readend;
                        }
                }               /* end of switch */
        }                       /* end of while */
      readend:
        return MRD_NONE;
}


static int
send_to_mr (s, pmsg)
SOCKET s;                       /* no use here */
char *pmsg;                     /* no use here */
{
        int len = 0;
        int index = 0;
        fd_set fdws, fdrs, fdes;

        index = 0;
        strcpy (m_buffer, m_qs);
        while (1)
        {
                FD_ZERO (&fdws);
                FD_SET (m_svrsock, &fdws);
                switch (select (m_svrsock + 1, NULL, &fdws, NULL, NULL))
                {
                case -1:
                case 0:
                        error (1, 0, "read socket error");
                        break;
                default:
                        if (FD_ISSET (m_svrsock, &fdws))
                        {
                                if (0 > (len = send (m_svrsock, &m_buffer[index], strlen (m_buffer), 0)))
                                {
                                        log (_T ("write error"));
                                        return -2;
                                }
                                /* if(len==(strlen(m_buffer)-index)) break; */
                                /* use goto here to jump out of the while(1)
                                   loop */
                                if (len == (strlen (m_buffer) - index))
                                {
                                        goto sendend;
                                }
                                index += len;
                                break;
                        }       /* end of if (FD_ISSET) */
                }               /* end of switch */
        }                       /* end of while */
      sendend:
        return MRD_NONE;
}

int
pcinit ()
{

        CVSMSG msg;
        int id, pid, *pint;
        int sock;
        struct sockaddr_in svraddr;

        if (0 > (m_svrsock = socket (AF_INET, SOCK_STREAM, 0)))
        {
                error (1, 0, "cannot create socket");
                return CANNOTSOCKET;
        }
        svraddr.sin_family = AF_INET;
        svraddr.sin_port = htons (SERVERPORT);
        svraddr.sin_addr.s_addr = inet_addr (SERVERIP);
        if (0 > connect (m_svrsock, (SADDR) & svraddr, sizeof (svraddr)))
        {
                error (1, 0, "Cannot connect to MR info server");
                return CANNOTCONNECT;
        }

        return 0;
}


void
buildinsertpara2 (filename, pathname, mrid, comment, lmtime, rev, tag)
char *filename;
char *pathname;
char *mrid;
char *comment;
char *lmtime;
char *rev;
char *tag;
{
        int i = 0;
        char buffer[1024];

        strcpy (buffer, mrid);
        while (i < strlen (buffer))
        {
                if (buffer[i] == ' ')
                        buffer[i] = '+';
                i++;
        }

        sprintf (m_qs, "%sID=%s&FILENAME=%s&PATHNAME=%s&MRID=%s&COMMENT=%s&REV=%s&TAG=%s %s", m_qh, buffer, filename, pathname, buffer, comment, rev, tag, m_qe);
        /* error (0, 0, m_qs); */
}



void
buildinsertpara (filename, pathname, mrid, comment, lmtime)
char *filename;
char *pathname;
char *mrid;
char *comment;
char *lmtime;
{
        int i = 0;
        char buffer[1024];

        strcpy (buffer, mrid);
        while (i < strlen (buffer))
        {
                if (buffer[i] == ' ')
                        buffer[i] = '+';
                i++;
        }

        sprintf (m_qs, "%sID=%s&FILENAME=%s&PATHNAME=%s&MRID=%s&COMMENT=%s %s", m_qh, buffer, filename, pathname, buffer, comment, m_qe);
        /* error (0, 0, m_qs); */
}

void
buildquerypara (username, mrid)
char *username;
char *mrid;
{
        int i = 0;
        char buffer[1024];

        strcpy (buffer, mrid);
        while (i < strlen (buffer))
        {
                if (buffer[i] == ' ')
                        buffer[i] = '+';
                i++;
        }


        sprintf (m_qs, "%sMRID=%s&LOGINNAME=%s&FASONG=SEND %s", m_qhid, buffer, username, m_qe);
        /* ACTION ("querystring for mrid", m_qs); */
}

/*
 refine context data from recieved data 
 */

char *
refinecontext (pc)
char *pc;
{
        char *p;
        unsigned long count = 0;

        p = pc;
        while (!(p[count] == '\r' && p[count + 1] == '\n' && p[count + 2] == '\r' && p[count + 3] == '\n'))
        {
                count += 1;
        }
        /* error (0, 0, &p[count+4]); */

        return &p[count + 4];
}

void
pcshut (void)
{
        close (m_svrsock);
}

/* out interface */
int
insertmr (filename, pathname, mrid, comment, lmtime)
char *filename;
char *pathname;
char *mrid;
char *comment;
char *lmtime;
{
        char *pr;
        int err = 0;

        memset (g_answer, 0, BUFMAX);

        if (0 > (err = pcinit ()))
                return err;

        buildinsertpara (filename, pathname, mrid, comment, lmtime);
        send_to_mr (0, m_tag);
        read_from_mr (0, g_answer);
        pcshut ();
        pr = refinecontext (&m_buffer[0]);
        // error (0, 0, pr);

        return (0);
}

int
insertmr2 (filename, pathname, mrid, comment, lmtime, rev, tag)
char *filename;
char *pathname;
char *mrid;
char *comment;
char *lmtime;
char *rev;
char *tag;
{
        char *pr;
        int err = 0;
        char *f;

        memset (g_answer, 0, BUFMAX);
        f = strrchr (filename, '/');
        if (NULL == f)
        {
                f = filename;
        }
        else
        {
                f += 1;
        }

        if (0 > (err = pcinit ()))
                return err;

        // buildinsertpara2 (filename, pathname, mrid, comment, lmtime, rev, tag);
        buildinsertpara2 (f, pathname, mrid, comment, lmtime, rev, tag);
        send_to_mr (0, m_tag);
        read_from_mr (0, g_answer);
        pcshut ();
        pr = refinecontext (&m_buffer[0]);
        // error (0, 0, pr);

        return (0);
}



int
querymrid (username, mrid)
char *username;
char *mrid;
{
        char *pr;
        int err = 0;

        if (0 > (err = pcinit ()))
                return err;

        buildquerypara (username, mrid);
        send_to_mr (0, m_tag);
        read_from_mr (0, g_answer);
        pr = refinecontext (&m_buffer[0]);
        pcshut ();
        // ACTION ("query result", pr);
        if ('Y' == pr[0])
        {
                return (0);
        }
        else
        {
                return (1);
        }
}


void
buildupdatepara (mrid, pathname, tag, branch)
char *mrid;
char *pathname;
char *tag;
char *branch;
{
        int i = 0;
        char buffer[1024];

        strcpy (buffer, mrid);
        while (i < strlen (buffer))
        {
                if (buffer[i] == ' ')
                        buffer[i] = '+';
                i++;
        }


        sprintf (m_qs, "%sMRID=%s&PATHNAME=%s&TAG=%s&BRANCH=%s %s", m_utag, buffer, pathname, tag, branch, m_qe);
        /* ACTION ("querystring for mrid", m_qs); */
}


/*
 * branch is real TAG name here!!!!!!!!!!!!!
 */
int
updatetag (mrid, pathname, tag, branch)
char *mrid;
char *pathname;
char *tag;
char *branch;
{
        char *pr;

        if (NULL == mrid || NULL == pathname || NULL == tag)
                return 0;

        pcinit ();
        buildupdatepara (mrid, pathname, tag, branch);
        send_to_mr (0, m_tag);
        pcshut ();
/*************
	read_from_mr (0, g_answer);
	pr = refinecontext (&m_buffer[0]);
	pcshut ();
	// ACTION ("query result", pr);
	if ('Y' == pr[0])
	{
		return (0);
	}else{
		return(1);
	}
*/
}


int
deny_or_allow (char *username)
{
        int err = 0;

        return err;
}

#endif
