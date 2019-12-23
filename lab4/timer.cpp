#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static int pid;
static int usr_sig;
static timer_t timerid;

static void usrsignal_handler(int sig, siginfo_t *si, void *uc) {
    kill(pid, usr_sig);
}

static void timerdel_handler(int s) {
    timer_delete(timerid);
    exit(1);
}

int main(int argc, char *argv[]) {
    struct sigevent sev;
    struct itimerspec its;
    int freq_secs;

    int year, month, day, hour, minute, second = 0;
    struct tm calendar_time;
    time_t time_from_epoch;

    struct sigaction sa;
    char checker;

    signal(SIGINT, timerdel_handler);

    if (argc != 3) {
        fprintf(stderr, "%s <process> <signal>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    pid = atoi(argv[1]);
    usr_sig = atoi(argv[2]);

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = usrsignal_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
        errExit("timer_create");

    printf("Do you want to call it once? <y>/<n>\n");
    checker = getchar();

    if (checker == 'n') {
        printf("Seconds: ");
        scanf("%i", &freq_secs);

        its.it_value.tv_sec = freq_secs;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;

        if (timer_settime(timerid, 0, &its, NULL) == -1)
            errExit("timer_settime");

    } else {
        printf("Time: <year>:<month>:<day>:<<hour>:<minute>:<second>");
        scanf("%d:%d:%d:%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

        calendar_time.tm_year = year - 1900;
        calendar_time.tm_mon = month - 1;
        calendar_time.tm_mday = day;
        calendar_time.tm_hour = hour;
        calendar_time.tm_min = minute;
        calendar_time.tm_sec = second;
        calendar_time.tm_isdst = -1;
        time_from_epoch = mktime(&calendar_time);

        //time(&time_from_epoch);
        //printf ("%ld %ld", (long) time_from_epoch, time_from_epoch);

        its.it_value.tv_sec = (long) time_from_epoch;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;

        if (timer_settime(timerid, TIMER_ABSTIME, &its, NULL) == -1)
            errExit("timer_settime");
    }

    printf("Timer were launched. Press Ctrl-c to delete timer and quit");
    while (1);

    exit(EXIT_SUCCESS);
}
