/*
 * linux user space timer list
 * Copyright (c) 2015, roamingunner <103922926@qq.com>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include "list.h"

typedef void (*timer_handle)(void *user_ctx);

struct timer{
    struct timeval timeout;
    void *user_ctx;
    timer_handle handle;
    struct dl_list list;
};



inline int time_before(struct timeval *a, struct timeval *b);
inline int get_time(struct timeval *t);
inline void time_sub(struct timeval *a, struct timeval *b,struct timeval *res);
inline void time_age(struct timeval *start,struct timeval *age);
inline int time_initialized(struct timeval *t);

struct timer *register_reltimer(unsigned int second, unsigned int microsecond,timer_handle handle,void *user_ctx);

int cancel_timer(struct timer *tr);

void cancel_timer_list(void);

void display_timer_list(void);

#endif
