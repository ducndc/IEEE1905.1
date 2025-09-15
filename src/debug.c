/*
 * debug.c - for debug and logging
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 *
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "debug.h"

extern const char *PROG_NAME;

static int loglevel;
static uint32_t features;
static const char *logfile;
static FILE *outfile;
static int ffd = -1;
static int ofd = -1;
static bool syslogging;
static bool logfile_isfifo;
static const int syslog_level[] = { EMERGENCY, ERROR, WARN, INFO, DEBUG };

uint32_t 
logfeature_to_enum(
    const char *s)
{
	if (!strncmp(s, "MAIN", 4)) {
		return BIT(LOG_MAIN);
    }

	if (!strncmp(s, "TIMER", 5)) {
		return BIT(LOG_TIMER);
    }

	if (!strncmp(s, "NEIGH", 5)) {
		return BIT(LOG_NEIGH);
    }

	if (!strncmp(s, "WSC", 3)) {
		return BIT(LOG_WSC);
    }

	if (!strncmp(s, "CMD", 3)) {
		return BIT(LOG_CMD);
    }

	if (!strncmp(s, "TX", 2)) {
		return BIT(LOG_TX);
    }

	if (!strncmp(s, "RX", 2)) {
		return BIT(LOG_RX);
    }

	if (!strncmp(s, "FRAG", 4)) {
		return BIT(LOG_FRAG);
    }

	if (!strncmp(s, "DM", 2)) {
		return BIT(LOG_DM);
    }

	if (!strncmp(s, "DEFAULT", 7)) {
		return BIT(LOG_DEFAULT);
    }

	if (!strncmp(s, "ALL", 3)) {
		return LOG_FEATURE_ALL;
    }

	return 0;
}

const char *
logfeature_to_string(
    uint32_t e)
{
	switch (e) {
	case LOG_MAIN:    return "MAIN";
	case LOG_TIMER:   return "TIMER";
	case LOG_NEIGH:   return "NEIGH";
	case LOG_CMDU:    return "CMDU";
	case LOG_WSC:     return "WSC";
	case LOG_CMD:     return "CMD";
	case LOG_TX:      return "TX";
	case LOG_RX:      return "RX";
	case LOG_FRAG:    return "FRAG";
	case LOG_DM:      return "DM";
	case LOG_DEFAULT: return "DEFAULT";
	}

	return "";
}

// void 
// restart_logging(
//     void *args)
// {
// 	struct ieee_1905_user_opts *opts = args;

// 	syslogging = opts->sys_logging;
// 	logfile = opts->log_file;
// 	logfile_isfifo = opts->log_file_is_fifo;
// 	loglevel = opts->log_level;
// 	features = opts->features;

// 	if (syslogging) {
// 		openlog(PROG_NAME, 0, LOG_DAEMON);
//     }

// 	if (!logfile) {
// 		outfile = stderr;
// 		ofd = fileno(stderr);
// 		return;
// 	}

// 	if (logfile_isfifo) {
// 		struct stat st;
// 		int rfd;

// 		if (stat(logfile, &st)) {
// 			unlink(logfile);
//         }

// 		mkfifo(logfile, 0600);

// 		if ((stat(logfile, &st) == -1) || !S_ISFIFO(st.st_mode)) {
// 			return;
//         }

// 		rfd = open(logfile, O_RDONLY | O_NONBLOCK);

// 		if (rfd) {
// 			ffd = open(logfile, O_WRONLY | O_NONBLOCK);
// 			close(rfd);
// 		}
// 	} else {
// 		ofd = open(logfile, O_CREAT | O_WRONLY | O_APPEND | O_NONBLOCK);
// 	}
// }

void 
stop_logging(
    void)
{
	if (syslogging) {
		closelog();
    }

	if (outfile) {
		fclose(outfile);
    }

	if (ofd > 0) {
		close(ofd);
		ofd = -1;
	}

	if (ffd > 0) {
		close(ffd);
		unlink(logfile);
	}
}

static void 
log_timestamp(
    int fd)
{
	time_t now = time(NULL);
	struct tm *tm_now = localtime(&now);
	const char *tm_fmt = "[%d-%02d-%02d %02d:%02d:%02d] ";

	dprintf(fd, tm_fmt,
		tm_now->tm_year + 1900,
		tm_now->tm_mon + 1,
		tm_now->tm_mday,
		tm_now->tm_hour,
		tm_now->tm_min,
		tm_now->tm_sec);
}

void 
log_message(
    int feature, 
    int level, 
    const char *fmt, 
    ...)
{
	va_list args;
	int fd = -1;

	if (!(BIT(feature) & features)) {
		return;
    }

	if (level > loglevel) {
		return;
    }

	va_start(args, fmt);

	if (syslogging && level >= 0) {
		vsyslog(syslog_level[level > 3 ? 3 : level], fmt, args);
    }

	if (logfile_isfifo && ffd > 0) {
		fd = ffd;
    } else if (ofd >= 0) {
		fd = ofd;
    }

	if (fd != -1) {
		log_timestamp(fd);
		dprintf(fd, "[%d]: ", getpid());
		vdprintf(fd, fmt, args);
	}

	va_end(args);
}

void 
log_cmdu(
    uint8_t *buf, 
    size_t buflen, 
    const char *ifname, 
    bool is_rx)
{
	const char *fmt = "{\"type\":\"%s\", \"ifname\":\"%s\", \"cmdu\":\"%s\"}";
	const char *flag = getenv("IEEE1905_LOG_CMDU");
	size_t len = 256 + 3028;
	char cmdu[3028] = { 0 };
	char *endptr = NULL;
	int f_val;

	if (!flag) {
		return;
    }

	errno = 0;
	f_val = strtol(flag, &endptr, 10);

	if (errno || *endptr != '\0') {
		fprintf(stderr, "Invalid IEEE1905_LOG_CMDU value: %s\n", flag);
		return;
	}

	if (f_val == 0) {
		return;
    }

	if (buf && buflen) {
		btostr(buf, buflen, cmdu);
    }

	if (ifname) {
		len += strlen(ifname);
    }

	char jdata[len];
	snprintf(jdata, sizeof(jdata), fmt, is_rx ? "rx" : "tx", (ifname ? ifname : ""), (buf ? cmdu : ""));
	log_message(LOG_CMDU, LOG_ERR, "%s\n", jdata);
}