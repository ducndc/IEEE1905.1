/*
 * timer.h
 * Header file for timer utility functions.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <libubox/uloop.h>

typedef struct atimer {
	/* uloop timer implementation */
	struct uloop_timeout t;
	struct timeval expires;
} atimer_t;

void timer_init(atimer_t *t, void (*function)(atimer_t *));
int timer_set(atimer_t *t, uint32_t tmo_ms);
int timer_del(atimer_t *t);
int timer_pending(atimer_t *t);
int timer_remaining_ms(atimer_t *t);

 #endif /* END _TIMER_H_ */