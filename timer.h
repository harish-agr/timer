/*
 * linux user space timer list
 * Copyright (c) 2015, roamingunner <103922926@qq.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

typedef void (*timer_hander)(void *user_ctx);

struct timer{
    struct timeval timeout;
    void *user_ctx;
    timer_hander hander;
    struct dl_list list;
}


inline int time_before(struct timeval *a, struct timeval *b)
{
    return (a->tv_sec < b->tv_sec) ||
           (a->tv_sec == b->tv_sec && a->tv_usec < b->tv_usec);
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


/* Helpers for handling struct reltime */

inline int time_before(struct timeval *a,
                struct timeval *b)
{
    return (a->tv_sec < b->tv_sec) ||
            (a->tv_sec == b->tv_sec && a->tv_usec < b->tv_usec);
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

#endif
