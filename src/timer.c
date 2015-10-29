#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

static DEFINE_DL_LIST(timer_list_header);

inline int time_before(struct timeval *a, struct timeval *b)
{
    return (a->tv_sec < b->tv_sec) ||
        ((a->tv_sec == b->tv_sec) && (a->tv_usec < b->tv_usec));
}

inline int get_time(struct timeval *t)
{
    int res;
    res = gettimeofday(t, NULL);
    return res;
}

inline void time_sub(struct timeval *a, struct timeval *b,
            struct timeval *res)
{
    res->tv_sec = a->tv_sec - b->tv_sec;
    res->tv_usec = a->tv_usec - b->tv_usec;
    if (res->tv_usec < 0) {
        res->tv_sec--;
        res->tv_usec += 1000000;
    }
}

inline void time_age(struct timeval *start,
                struct timeval *age)
{
    struct timeval now;

    get_time(&now);
    time_sub(&now, start, age);
}

/* inline int time_expired(struct timeval *now, */
/* 				struct timeval *ts,time_t timeout_secs) */
/* { */
/* 	struct timeval age; */

/* 	time_sub(now, ts, &age); */
/* 	return (age.sec > timeout_secs) || */
/* 			(age.sec == timeout_secs && age.tv_usec > 0); */
/* } */


inline int time_initialized(struct timeval *t)
{
    return t->tv_sec != 0 || t->tv_usec != 0;
}

static void alarm_entry(int sig)
{
    struct timeval now;
    struct timer *t,*p;
    int ret;
    if ((sig != SIGALRM) ||
        dl_list_empty(&timer_list_header)){
        return ;
    }

    get_time(&now);

    dl_list_for_each_safe(t,p,&timer_list_header,struct timer,list){
        if (time_before(&t->timeout, &now)) {
            dl_list_del(&t->list);
            /* execute overtime timer callback */
            timer_handle handle = t->handle;
            handle(t->user_ctx);
            free(t);
        }else{
            /* timer in list. setup alarm again */
            struct itimerval alarm;
            struct timeval res;

            get_time(&now);
            memset(&alarm, 0, sizeof(struct itimerval));
            time_sub(&t->timeout,&now,&res);
            alarm.it_value.tv_sec = res.tv_sec;
            alarm.it_value.tv_usec = res.tv_usec;
            ret = setitimer(ITIMER_REAL,&alarm,NULL);
            if (ret){
                printf("[%s:%d]setup timer failed, %d\n",__func__,__LINE__,errno);
            }
            break;
        }
    }

    return ;
}

struct timer *register_reltimer(unsigned int second, unsigned int microsecond,timer_handle handle,void *user_ctx)
{
    struct timeval now,res;
    struct timer *tr,*p;
    struct itimerval alarm;
    int ret;

    if (!handle){
        return NULL;
    }

    get_time(&now);

    tr = malloc(sizeof(struct timer));
    if(tr){
        memset(tr,0,sizeof(struct timer));
    }else{
        return NULL;
    }
    tr->timeout.tv_usec = now.tv_usec + microsecond;
    tr->timeout.tv_sec = now.tv_sec + second;
    if (tr->timeout.tv_usec > 1000000){
        tr->timeout.tv_usec -= 1000000;
        tr->timeout.tv_sec++;
    }
    tr->handle = handle;
    tr->user_ctx = user_ctx;

    if (!dl_list_empty(&timer_list_header)){
        /* Maintain timeouts in order of increasing time */
        dl_list_for_each(p,&timer_list_header,struct timer,list){
            if (time_before(&tr->timeout, &p->timeout)) {
                /* reset alarm */
                dl_list_add(p->list.prev, &tr->list);
                break;
            }
        }
    }else{
        /* add first TIMER */
        signal(SIGALRM, alarm_entry);
        dl_list_add_tail(&timer_list_header, &tr->list);
    }

    /* get first timer, and setup alarm */

    memset(&alarm,0,sizeof(struct itimerval));

    p = dl_list_first(&timer_list_header,struct timer, list);

    get_time(&now);
    time_sub(&p->timeout,&now,&res);
    alarm.it_value.tv_sec = res.tv_sec;
    alarm.it_value.tv_usec = res.tv_usec;

    ret = setitimer(ITIMER_REAL,&alarm,NULL);
    if (ret){
        printf("Setup Timer failed!! errno %d\n",errno);
        free(tr);
        return NULL;
    }

    return tr;
}

int cancel_timer(struct timer *tr)
{
    struct timer *t,*p;
    if (!dl_list_empty(&timer_list_header)){
        dl_list_for_each_safe(t,p,&timer_list_header,struct timer,list){
            if(t == tr){
                dl_list_del(&t->list);
                free(t);
                return 0;
            }
        }
    }
    return -EINVAL;
}

void cancel_timer_list(void)
{
    struct timer *t,*p;
    if (!dl_list_empty(&timer_list_header)){
        dl_list_for_each_safe(t,p,&timer_list_header,struct timer,list){
            cancel_timer(t);
        }
    }
}


#if 1

void display_timer_list(void)
{
    struct timer *t;
    int i = 0;

    dl_list_for_each(t,&timer_list_header,struct timer, list){
        i++;
        printf("timer[%d] %p; timeout=%u:%u func=%p user_ctx=%p\n",
            i,t,
            (unsigned int)t->timeout.tv_sec,
            (unsigned int)t->timeout.tv_usec,t->handle,
            t->user_ctx);
    }
    return ;


}

#endif
