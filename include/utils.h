/*
 * util.h
 * implements utility functions and definitions.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <libubox/list.h>
#include <arpa/inet.h>
#include <sys/time.h>

extern int uuid_strtob(char *uuid_str, uint8_t *uuid);
extern int uuid_btostr(uint8_t *uuid, char *uuid_str);

struct ieee_1905_user_opts {
	const char *ubus_sock_path;
	const char *cmd_path;
	const char *pid_file;
	const char *obj_name;
	bool daemonize;
	const char *conf_file;
	const char *conf_path;
	uint32_t features;
	int log_level;
	const char *log_file;
	bool log_file_is_fifo;
	bool sys_logging;
	const char *al_id;
	bool lo;
	bool registrar;
};

/* default options */
#define IEEE1905_CONFFILE             "ieee1905"
#define IEEE1905_CONFFILE_PATH        "/etc/config"
#define IEEE1905_OBJECT               "ieee1905"
#define IEEE1905_OBJECT_EXT           IEEE1905_OBJECT".extension"
#define IEEE1905_PIDFILE              "/var/run/"IEEE1905_OBJECT".pid"
#define IEEE1905_CMDPATH              "/tmp/ieee1905"

#define MACFMT		"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(_m)	(_m)[0], (_m)[1], (_m)[2], (_m)[3], (_m)[4], (_m)[5]
#define MACSTR MACFMT

void do_daemonize(const char *pid_file);
int timeradd_msecs(struct timeval *a, unsigned long msecs, struct timeval *res);
void get_random_bytes(int num, uint8_t *buf);
void bufprintf(uint8_t *buf, int len, const char *label);

typedef unsigned int ifstatus_t;
typedef unsigned char ifopstatus_t;

enum if_mediatype {
	IF_MEDIA_ETH,
	IF_MEDIA_WIFI,
	IF_MEDIA_PLD,
	IF_MEDIA_MOCA,
	IF_MEDIA_UNKNOWN,
};

int if_br_port_num(const char *ifname);
int if_get_carrier(const char *ifname, int *carrier);

const char *regex_match(const char *str, const char *pattern);

#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

#ifndef max
#define max(a,b)	(a) > (b) ? (a) : (b)
#endif

#define MACFMT		"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(_m)	(_m)[0], (_m)[1], (_m)[2], (_m)[3], (_m)[4], (_m)[5]

#ifndef list_flush
#define list_flush(head, type, member)					\
do {									\
	type *__p = NULL, *__tmp;					\
									\
	if (!list_empty(head))						\
		list_for_each_entry_safe(__p, __tmp, head, member) {	\
			list_del(&__p->member);				\
			free(__p);					\
		}							\
} while (0)
#endif

#define foreach_token_r(t, tokens, saveptr, delim)	\
	for ((t = strtok_r(tokens, delim, &saveptr)); t; (t = strtok_r(NULL, delim, &saveptr)))

char *strstr_exact(char *haystack, const char *needle);

#define blobmsg_add_macaddr(b, f, v)	\
do {					\
	char _vstr[18] = {0};		\
	hwaddr_ntoa(v, _vstr);		\
	blobmsg_add_string(b, f, _vstr);\
} while (0)

/** Convert hex string to byte array
 *
 * For example:
 * "001122334455" --> {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
 *
 * @param[in] str hex string.
 * @param[in] len length of output buffer passed to the function.
 * @param[out] bytes output buffer to write the converted hex string.
 * @return byte array of the converted hex string, or %NULL on error.
 */
uint8_t *strtob(char *str, int len, uint8_t *bytes);

#endif /* END _UTILS_H_ */