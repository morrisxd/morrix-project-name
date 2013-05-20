#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <setjmp.h>
#include <libgen.h>
#include <assert.h>
#include <stdarg.h>
#include <syslog.h>

/*add for debug message write to syslog*/
static const unsigned int facility = LOG_LOCAL0;

int wddi_log_msg(const unsigned int priority, char *file, unsigned int line, char *log_msg, ...);
#define FILE_NOPATH (char *)(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define WDDI_WARNINGNL(...)                                                         \
do                                                                                 \
{                                                                                  \
  (void) wddi_log_msg(LOG_WARNING, FILE_NOPATH, __LINE__, "\n" __VA_ARGS__);  \
} while (0)

#define printf(fmt, ...)   WDDI_WARNINGNL(fmt,## __VA_ARGS__)


int
wddi_log_msg(const unsigned int priority, char *file, unsigned int line, char *log_msg, ...)
{

   va_list args;
   va_start(args, log_msg);       /*lint !e530 varargs */
   char log_entry_str[1024];
   int cnt = 0;
   cnt = snprintf(log_entry_str, 1024, "%s", " ");

   if(cnt < 0)
   {
      cnt = 0;
   }

   (void)vsnprintf(&(log_entry_str[cnt]), 1024 - cnt, log_msg, args);

   /* Write into the specified syslog. */
   syslog(facility | priority, log_entry_str);

   va_end(args);

   return 0;
}



#ifndef O_NOFOLLOW
#define O_NOFOLLOW     00400000        /* don't follow links */
#endif


#define CMD_NAME "wdg"
#define WPL_SIGNAL_RT_SER 35 /*SIGRTMIN*/
#define WPL_SIGNAL_RT_OV (WPL_SIGNAL_RT_SER + 1)
#define WPL_SIGNAL_RT_BUSERR (WPL_SIGNAL_RT_SER + 2)
#define SIGNAL_RT_SGMII_AN           45
#define SIGNAL_RT_UFE4_FRAMER        46


static sigjmp_buf                   jmpbuf;
pid_t g_childPid = 0;

int getpath(char *buf)
{
long size;
char *ptr;
int ret;
size = pathconf(".", _PC_PATH_MAX);
if((ptr = (char *)malloc((size_t)size)) != NULL)
  {
  memset(ptr,0,size);
  sprintf(ptr,"/proc/%d/exe",getpid());
  }else return -1;
ret=readlink(ptr,buf,size);
free(ptr);
return ret;
}


pid_t getpidbyname(const char *name)
{
        char buf[128];
        FILE *fp = tmpfile();

        assert(name);

        /* The buf save a shell command that take pid to tmpfile */
        //sprintf(buf, "ps -A | awk '/.*%s.*/{print $1}' >&%d",
        sprintf(buf, "ps -A | awk '/ %s$/{print $1}' >&%d",
                name, fileno(fp));
        /* Run the command */
        if (-1 == system(buf))
        {
                perror("system ERROR");
                //exit(1);
                fclose(fp);
                return -1;
        }

        /* Get pid from tmpfile */
        fseek(fp, 0, SEEK_SET);
        if (NULL == fgets(buf, sizeof buf, fp))
        {
                //perror("fgets-ERROR");
                fclose(fp);
                return -1;
        }
		fclose(fp);
        return atoi(buf);
}


static void
print_help(void)
{
    printf(CMD_NAME " [-hrnl]\n");
    printf("  -h       : This help text.\n");
    printf("  -r   :   wddi server path to be monitored.\n");
    printf("  -n   :   Arguments to server -n DEBUG LEVEL .\n");
    printf("  -l   :   xml file loaction default:/etc/spp/active     \n");
}

void
print_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n",
                WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d\n",
                WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n",
                WSTOPSIG(status));
}

static void sig_sigterm(int signo)
{
   if (signo != SIGTERM)
   {
      return;
   }
   //printf("Receive SIGTERM event\n");
   kill(0,SIGINT);/*kill all process in this group ,include itself*/
   return;
}

int main(int argc, char **argv)
{
    pid_t child_pid;
    int ret;
    int serverfd;
    char sevpath[100];
    char LogLevel[100];
	char workingLinkName[100];
	char *wdgname = NULL;
	char xmlpath[100];
	char *appname = NULL;
	int servpath_flag = 0;
	int LogLevel_flag  = 0;
	int xmlpath_flag = 0;
	pid_t appId;
    int opt;

	sigset_t sigmask;
	 //pid_t chldpid = child_pid;
	 sigemptyset(&sigmask);
	 struct sigaction siginfo;

	 sigaddset(&sigmask,  SIGALRM |WPL_SIGNAL_RT_SER | WPL_SIGNAL_RT_OV |WPL_SIGNAL_RT_BUSERR|SIGNAL_RT_SGMII_AN|SIGNAL_RT_UFE4_FRAMER);
	 sigprocmask(SIG_BLOCK, &sigmask, NULL);


	while((opt = getopt(argc, argv, "hr:n:l:"))!= -1) {
		switch(opt){
		case 'h':
			print_help();
			return -1;
	    case 'r':
			sprintf(sevpath,"%s",optarg);
			servpath_flag = 1;
			break;
		case 'n':
			sprintf(LogLevel,"%s",optarg);
			LogLevel_flag = 1;
			break;
		case 'l':
			sprintf(xmlpath,"%s",optarg);
			xmlpath_flag = 1;
			break;
		default:
			printf("Unknown option \n");
			return -1;
		}
	  }
    if(servpath_flag == 0)
    {
		printf("-r server path needed -l:\n");
		print_help();
	}


    /* Check that a wddi server path is supplied.
     */

    /* no binary given, we have to reboot */
   if((serverfd = access (sevpath,X_OK)) != 0)
   {
      printf("%s doesn't exist or has no execute privilige.\n", sevpath);
      return (errno);
   }

   appname = basename(sevpath);
   if(appname != NULL)
   {
	  printf("application name is %s \n",appname);
   }
   else
   {
	  printf("invalid application name %s \n",sevpath);
   }

   if (sigsetjmp(jmpbuf, 1)) {
        printf("repeat_create\n");
    }

repeat_create:

    while(1)/*scan other wdg is run*/
    {
		if(getpath(workingLinkName) == -1)
		{
		    printf("get wdg name fail \n");
		}
		wdgname =basename(workingLinkName);
		appId = getpidbyname(wdgname);
        if(appId < 0)
		{
            break;
		}
		if(appId == getpid())break;
		printf("wdgwrapper %s existed ,kill it pid = %d \n",wdgname,appId);
		kill(appId, SIGKILL);
		usleep(500000);

	}

	/*scan application alread existed ,if there is kill them*/
	while(1)
	{
		waitpid(-1,NULL,WNOHANG );/*in case of when child exited ,generate defunct process*/
		appId = getpidbyname(appname);
		if(appId < 0)
		{
		   //printf("can't get application %s\n",appname);
		   break;
		}
		/*kill application*/
		//printf("application %s existed ,kill it pid = %d \n",appname,appId);
		//kill(appId, SIGKILL);
		usleep(500000);
	}
	  sleep(15);
    printf("vfork................\n");
    child_pid = vfork();
    if (!child_pid) {

	/* Don't want the stdin and stdout of our program to cause trouble.
	 *
	 * So make stdout and stderr go to their respective files */
        /* now start binary */
      //close(heartfd[0]);
	if (LogLevel_flag ==0 && xmlpath_flag ==0)
		{
		printf("LogLevel = NULL xmlpath = NULL\n");
		execl(sevpath,sevpath, NULL, NULL);

	     }
	else if (LogLevel_flag == 0 && xmlpath_flag !=0)
		{
		printf("LogLevel = NULL xmlpath = %s\n",xmlpath);

		execl(sevpath,sevpath, "-l", xmlpath, NULL);

	    }
	else if(LogLevel_flag != 0 && xmlpath_flag !=0)
		{
		printf("LogLevel =%s xmlpath =%s \n \n",LogLevel,xmlpath);

		execl(sevpath,sevpath,"-n", LogLevel, "-l", xmlpath, NULL);

	    }
	else if (LogLevel_flag != 0 && xmlpath_flag ==0)
	{
		printf("LogLevel = %s xmlpath = NULL \n \n",LogLevel);

		execl(sevpath,sevpath, "-n", LogLevel, NULL);
	}
	else
		{
            printf("invalid parameters \n");
			sleep(1);
		    exit( 1 );
	    }

	/* execl should only return in case of an error */
	/* so we return the reboot code */
	return (errno);
    } else if (child_pid < 0) {	/* fork failed */
	perror("fork");
	return (errno);
    }


   sigaction(SIGTERM, NULL, &siginfo);
   siginfo.sa_sigaction = (void *) sig_sigterm;
   sigemptyset(&siginfo.sa_mask);
   siginfo.sa_flags = SA_SIGINFO | SA_RESTART;

   if (sigaction(SIGTERM, &siginfo, NULL) == -1){
		printf("can't catch SIGTERM");
   }
   sleep(1);

   while(1)
   {
           ret = waitpid(-1,NULL,WNOHANG );/*in case of when child exited ,generate defunct process*/
           if(ret != 0)
           {
               if((appId=getpidbyname(appname))<0)
               {
                   g_childPid = 0;
                   printf("child(%d) has been exited,go to repeat_create,ret=%d\n",child_pid,ret);
                   sleep(2);
                   goto repeat_create;
               }   
           }
      sleep(1);
   }

   int waitloop = 90;//it must wait a long time so that child can be started.
   while(waitloop-- > 0 )
   {
	   ret = waitpid(-1,NULL,WNOHANG );/*in case of when child exited ,generate defunct process*/
	   if(ret != 0)
	   {
		   g_childPid = 0;
		   printf("child has been exited,go to repeat_create\n");
		   sleep(2);
		   goto repeat_create;
	   }
      sleep(1);
   }


   g_childPid = child_pid;
   /*keep detect spp_npu,if not existed create it*/
   while(1)
   {
	   waitpid(-1,NULL,WNOHANG );/*in case of when child exited ,generate defunct process*/
	   appId = getpidbyname(appname);
	   if(appId < 0)
	   {
		  printf("application %s not existed,restarted it\n",appname);
		  goto repeat_create;
	   }
	   sleep(1);
   }

  return 1;

}

