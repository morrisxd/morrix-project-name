#ifdef __linux__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/times.h> 
#include <signal.h>
#include <errno.h>


#include "ce.h"

#define SIG             (SIGRTMIN + 3)
#define CLOCKID         CLOCK_REALTIME

static timer_t timerid;

extern int aging_sem;
extern unsigned int aging_period;

/* Timer handler for Aging, start the timer periodically */
static void handler(int sig, siginfo_t *si, void *uc)
{
        struct itimerspec its;

        WPL_SemIncrement(&aging_sem, 1);
        
        its.it_value.tv_sec = aging_period;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
        if (timer_settime(timerid, 0, &its, NULL) == -1)
                return ;
}


/* Create the timer, using the standard linux timer and signal */
int OS_AgingTimerCreate(void)
{
        struct sigaction sa;
        struct sigevent sev;
        
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIG, &sa, NULL) == -1)
                return -1;

        sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIG;
        sev.sigev_value.sival_ptr = &timerid;
        if (timer_create(CLOCKID, &sev, &timerid) == -1)
                return -1;

        return 0;
}

/* Start the timer, and we decrease the weight firstly */
int OS_AgingTimerStart(void)
{
        struct itimerspec its;

        its.it_value.tv_sec = aging_period;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;

        WPL_SemIncrement(&aging_sem, 1);
        
        if (timer_settime(timerid, 0, &its, NULL) == -1)
                return -1;

        return 0;
}

/* Delete the timer */
int OS_AgingTimerDelete(void)
{
        if (timer_delete(timerid))
                return -1;
        
        return 0;
}

#else

static WP_THREAD_ID timer_tid;
static WP_SEM_ID timer_sem;
static WP_U32 timer_enable;

extern void * App_AgingFunc(void *arg);

#define ssnop()     __asm__ __volatile__("sll   $0, $0, 1\n");
#define ssnop_4()   do { ssnop(); ssnop(); ssnop(); ssnop(); } while (0)


void * OS_TimerThread(void *arg)
{
        WP_U32 start = 0, current = 0;
        WP_U32 delta = 0;
        int cntr = 0;
                
        while (1)
        {
                // Maybe WinMon semaphore will be supported later, it is so important
                WPL_SemDecrement(&timer_sem, 1);
                
                start = WT_TimeRead();
                delta = 0;

                while (1)
                {
                        // delay
                        for (cntr = 0; cntr < 100; cntr++)
                                ssnop_4();

                        if (!timer_enable) break;

                        // read timestamp
                        current = WT_TimeRead();
                        delta  += WT_TimeDelta(current, start);
                        
                        if (aging_enable && (delta >= aging_period * 1000 * 1000))
                        {
                                App_AgingFunc(NULL);
                                break;
                        }
                        
                        start   = current;
                }                

                // again or not
                if (timer_enable) 
                {
                        WPL_SemIncrement(&timer_sem, 1);
                }
        }
}

int OS_AgingTimerCreate(void)
{
        static int flag = 0;
        
        if (!flag)
        {
                timer_enable = 0;

                WPL_SemInit(&timer_sem);
                WPL_ThreadInit(&timer_tid, OS_TimerThread, NULL);

                flag = 1;
        }
        
        return 0;
}

int OS_AgingTimerStart(void)
{
        timer_enable = 1;
        WPL_SemIncrement(&timer_sem, 1);
        App_AgingFunc(NULL);
        
        return 0;
}

int OS_AgingTimerDelete(void)
{
        timer_enable = 0;
        
        return 0;
}

#endif

