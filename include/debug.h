/*
 * debug.h - header file for debug and log messages.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

/*****************************************************************************************************
 * 									MACRO DECLEARATION												 *
 *****************************************************************************************************/

#define LOG_FEATURE(x)	BIT(x)
#define LOG_FEATURE_ALL	BIT(LOG_DEFAULT) | (BIT(LOG_DEFAULT) - 1)

#define DEFAULT_LOGLEVEL	INFO

#define EM_ERR(fmt, ...)         log_message(LOG_DEFAULT, 0, fmt, ## __VA_ARGS__)
#define EM_WARN(fmt, ...)        log_message(LOG_DEFAULT, 1, fmt, ## __VA_ARGS__)
#define EM_INFO(fmt, ...)        log_message(LOG_DEFAULT, 2, fmt, ## __VA_ARGS__)
#define EM_DBG(fmt, ...)         log_message(LOG_DEFAULT, 3, fmt, ## __VA_ARGS__)
#define EM_TRACE(fmt, ...)       log_message(LOG_DEFAULT, 4, fmt, ## __VA_ARGS__)

#define EM_LOG_CMDU(b,l,i,t)      log_cmdu(b,l,i,t)

#define EM_WARN_ON(condition)					\
({								\
	int __rc = !!(condition);				\
	if (__rc)						\
		EM_WARN("%s: WARN '%s'\n", __func__, #condition);	\
	__rc;							\
})

/*****************************************************************************************************
 * 							      STRUCTURE DECLEARATION											 *
 *****************************************************************************************************/

/* log feature categories */
enum {
	LOG_MAIN,	/* Main process */
	LOG_TIMER,	/* Timers */
	LOG_NEIGH,	/* Neigh table entries */
	LOG_CMDU,	/* CMDU buffer dump in JSON format */
	LOG_WSC,	/* WSC and PBC */
	LOG_CMD,	/* CLI cmd request and response */
	LOG_TX,		/* Transmit */
	LOG_RX,		/* Receive */
	LOG_FRAG,	/* CMDU Fragmentation and Defragmentation */
	LOG_DM,         /* Data-Model related */
	LOG_DEFAULT,	/* uncategorized messages */
};

/* log levels */
enum {
    EMERGENCY,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE,
};

/*****************************************************************************************************
 * 								      FUNCTION DECLEARATION											 *
 *****************************************************************************************************/

uint32_t 
logfeature_to_enum(
	const char *s);

const char *
logfeature_to_string(
	uint32_t e);

void 
restart_logging(
	void *opts);

void 
stop_logging(
	void);

void 
log_message(
	int feature, 
	int level, 
	const char *fmt, 
	...)
	__attribute__((__format__(__printf__, 3, 4)));

void 
log_cmdu(
	uint8_t *buf, 
	size_t len, 
	const char *ifname, 
	bool is_rx);

#endif /* END _DEBUG_H_ */