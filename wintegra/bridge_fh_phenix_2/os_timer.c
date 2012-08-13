#ifdef __linux__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/times.h> 
#include <signal.h>
#include <errno.h>

#include "provider_bridge.h"

#define SIG             (SIGRTMIN + 3)
#define CLOCKID         CLOCK_REALTIME

static timer_t timerid;

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

#endif
