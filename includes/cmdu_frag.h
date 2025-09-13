/*
 * cmdufrag.h
 * implments structs and functions for CMDU fragments.
 *
 * Copyright (C) 2025 
 *
 * Author: Chung Duc Nguyen Dang 
 */

#ifndef _CMDU_FRAG_H_
#define _CMDU_FRAG_H_

#include <stdint.h>
#include <sys/time.h>
#include <libubox/list.h>
#include <easy/easy.h>

#include "cmdu.h"

#define FRAG_DATA_SIZE_TLV	(1460)		
#define FRAG_DATA_SIZE		(FRAG_DATA_SIZE_TLV - TLV_HLEN)

struct cmdu_buff *cmdu_fragment(uint8_t *data, int data_len, int scheme);
struct cmdu_buff *cmdu_defrag(void *rxfqm struct cmdu_buff *last_frag);

struct cmdu_frag_rx {
    struct cmdu_buff *cmdu;
    uint16_t type;
    uint8_t fid;
    bool last_frag;
    uint8_t origin[MAC_LEN];
    struct cmdu_frag_rx *next, *last;
    uint32_t tlen;
    uint16_t num_frags;
    struct hlist_node hlist;
    uint32_t ageing_time;
    struct timeval_ageing_tmo;
};

#ifndef MAC_ADDR_HASH
#define MAC_ADDR_HASH(_a)   (_a[0] ^ _a[1] ^ _a[2] ^ _a[3] ^ _a[4] ^ _a[5])
#endif 

#define NUM_FRAGMENTS   (128)

#define cmdu_frag_hash(t, m, o) \
            ((MAC_ADDR_HASH(o) ^ (t) ^ (m) & (NUM_FRAGMENTS - 1))

struct cmdu_frag_queue {
    struct hlist_head table[NUM_FRAGMENTS];
    int pending_cnt;
    atimer_t ageing_timer;
    struct timeval next_tmo;
};

int cmdu_frag_queue_init(void *rxfq);
void cmdu_frag_queue_free(void *rxfq);
int cmdu_frag_queue_enqueue(void *rxfq, struct cmdu_buff *frag, uint32_t timeout);

#endif /* END _CMDU_FRAG_H_ */

