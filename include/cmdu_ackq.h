/*
 * cmdu_ackq.h
 * CMDU pending response and ack queue management
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _CMDU_ACKQ_H_
#define _CMDU_ACKQ_H_

#include "buff_util.h"
#include "hlist.h"
#include "parameters.h"

/*****************************************************************************************************
 * 									MACRO DECLEARATION												 *
 *****************************************************************************************************/

#define CMDU_BACKLOG_MAX	(128)

#ifndef MAC_ADDR_HASH
#define MAC_ADDR_HASH(a)	(a[0] ^ a[1] ^ a[2] ^ a[3] ^ a[4] ^ a[5])
#endif

#define cmdu_ackq_hash(t, m, o)		\
	((MAC_ADDR_HASH(o) ^ (t) ^ (m)) & (CMDU_BACKLOG_MAX - 1))

#define CMDU_ACKQ_TMO_NONE	    (0)
#define CMDU_ACKQ_TMO_DELETE	(1)
#define CMDU_ACKQ_TMO_REARM	    (2)

/*****************************************************************************************************
 * 							      STRUCTURE DECLEARATION											 *
 *****************************************************************************************************/

/* struct holds a pending cmdu with 'mid' from 'origin' */
struct cmdu_ackq_entry {
	uint16_t type;
	uint16_t mid;
	uint8_t origin[MAC_LEN];
	void *cookie;
	struct hlist_node hlist;

	uint32_t ageing_time;    /* in msecs */
	struct timeval ageing_tmo;
	int resend_cnt;
};

/* queue of cmdus for which response is pending */
struct cmdu_ackq {
	/* hashtable of pending cmdu_ackq_entry */
	struct hlist_head table[CMDU_BACKLOG_MAX];

	int pending_cnt;
	atimer_t ageing_timer;
	struct timeval next_tmo;
	int (*timeout_cb) (struct cmdu_ackq *q, struct cmdu_ackq_entry *e);
	void (*delete_cb) (struct cmdu_ackq *q, struct cmdu_ackq_entry *e);
};

/*****************************************************************************************************
 * 								      FUNCTION DECLEARATION											 *
 *****************************************************************************************************/

extern int 
cmdu_ackq_init(
	void *q);

extern void 
cmdu_ackq_free(
	void *q);

extern void 
cmdu_ackq_flush(
	void *q);

extern int 
cmdu_ackq_dequeue(
	void *q, 
	uint16_t type, 
	uint16_t mid, 
	uint8_t *src, 
	void **cookie);

extern int 
cmdu_ackq_enqueue(
	void *q, 
	uint16_t type, 
	uint16_t mid, 
	uint8_t *dest, 
	uint32_t timeout, 
	int resend_cnt, 
	void *cookie);

extern struct 
cmdu_ackq_entry *
cmdu_ackq_lookup(
	void *cmdu_q, 
	uint16_t type, 
	uint16_t mid, 
	uint8_t *dest);

 #endif /* END _CMDU_ACKQ_H */