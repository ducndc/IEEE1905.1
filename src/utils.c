/*
 * util.c - implements utility functions.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <regex.h>
#include <dirent.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/route/link.h>
#include <netlink/route/addr.h>
#include <netlink/route/link/bridge.h>
#include <netlink/route/link/macvlan.h>

#include "buff_util.h"
#include "utils.h"

static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int hex2byte(const char *hex)
{
	int a, b;
	if((a = hex2num(*hex++)) < 0)
		return -1;
	if((b = hex2num(*hex++)) < 0)
		return -1;

	return (a << 4) | b;
}

#ifndef uuid_strtob

int uuid_strtob(char *uuidstr, uint8_t *uuid)
{
	uint8_t elen[] = {8, 4, 4, 4, 12};
	char *ptr, *end;
	uint8_t *ret;
	int idx = 0;
	int i = 0;

	if (!uuidstr || !uuid) {
		return -1;
	}

	ptr = uuidstr;
	end = uuidstr + strlen(uuidstr);

	while (ptr < end) {
		char tmp[32] = {0};
		char *pos;

		pos = ptr;
		ptr = strchr(pos, '-');

		if (!ptr) {
			if (i == 4) {
				ptr = end;
				if (ptr - pos != elen[i]) {
					return -1;
				}

				strncpy(tmp, pos, elen[i]);
				ret = strtob(tmp, elen[i] / 2, &uuid[idx]);
				return !ret ? -1 : 0;
			}

			return -1;
		}

		if (ptr - pos != elen[i]) {
			return -1;
		}

		strncpy(tmp, pos, elen[i]);
		ret = strtob(tmp, elen[i] / 2, &uuid[idx]);

		if (!ret) {
			return -1;
		}

		ptr++;
		idx += elen[i] / 2;
		i++;
	}

	return 0;
}

int uuid_btostr(uint8_t *uuid, char *uuidstr)
{
	if (!uuidstr || !uuid) {
		return -1;
	}

	sprintf(uuidstr,
		"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid[0], uuid[1], uuid[2], uuid[3],
		uuid[4], uuid[5], uuid[6], uuid[7],
		uuid[8], uuid[9], uuid[10], uuid[11],
		uuid[12], uuid[13], uuid[14], uuid[15]);

	return 0;
}

void do_daemonize(const char *pid_file)
{
	int f;

	if (daemon(0, 0)) {
		fprintf(stderr, "Can't becomes a daemon. "
			"Continue as foreground process...\n");
	}

	if (!pid_file) {
		return;
	}

	f = open(pid_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	if (f >= 0) {
		char buf[128] = {0};
		int flags;

		flags = fcntl(f, F_GETFD);

		if (flags != -1) {
			flags |= FD_CLOEXEC;
			fcntl(f, F_SETFD, flags);
		}

		if (lockf(f, F_TLOCK, 0) < 0) {
			fprintf(stderr, "File '%s' exists. Aborting...\n", pid_file);
			exit(-1);
		}

		if (ftruncate(f, 0)) {
			fprintf(stderr, "Continue with invalid pid file %s\n", pid_file);
			return;
		}

		snprintf(buf, sizeof(buf), "%ld\n", (long)getpid());

		if (write(f, buf, strlen(buf)) != (ssize_t)strlen(buf)) {
			fprintf(stderr, "Continue with invalid pid file %s\n", pid_file);
			return;
		}
	}
}

/* TODO: move following three functions to separate file */
int timeradd_msecs(struct timeval *a, unsigned long msecs, struct timeval *res)
{
	if (res) {
		struct timeval t = { 0 };

		if (msecs > 1000) {
			t.tv_sec += msecs / 1000;
			t.tv_usec = (msecs % 1000) * 1000;
		} else {
			t.tv_usec = msecs * 1000;
		}

		timeradd(a, &t, res);

		return 0;
	}

	return -1;
}

void get_random_bytes(int num, uint8_t *buf)
{
	unsigned int seed;
	struct timespec res = {0};
	int i;

	clock_gettime(CLOCK_REALTIME, &res);
	seed = res.tv_nsec;
	srand(seed);

	for (i = 0; i < num; i++) {
		buf[i] = rand_r(&seed) & 0xff;
	}
}

void _bufprintf(uint8_t *buf, int len, const char *label)
{
	int rows, residue;
	int i;
	int k;

	if (label) {
		fprintf(stderr, "---- %s ----\n", label);
	}

	rows = len / 16;

	for (k = 0; k < rows; k++) {
		fprintf(stderr, "\n   0x%08x | ", k * 16);

		for (i = 0; i < 16; i++) {
			if (!(i % 4)) {
				fprintf(stderr, "  ");
			}

			fprintf(stderr, "%02x ", buf[k*16 + i] & 0xff);
		}

		fprintf(stderr, "%8c", ' ');

		for (i = 0; i < 16; i++) {
			fprintf(stderr, "%c ", isalnum(buf[k*16 + i] & 0xff) ? buf[k*16 + i] : '.');
		}
	}

	residue = len % 16;
	k = len - len % 16;

	if (residue) {
		fprintf(stderr, "\n   0x%08x | ", rows * 16);

		for (i = k; i < len; i++) {
			if (!(i % 4)) {
				fprintf(stderr, "  ");
			}

			fprintf(stderr, "%02x ", buf[i] & 0xff);
		}

		for (i = residue; i < 16; i++) {
			if (!(i % 4)) {
				fprintf(stderr, "  ");
			}

			fprintf(stderr, "%s ", "  ");
		}

		fprintf(stderr, "%8c", ' ');

		for (i = k; i < len; i++) {
			fprintf(stderr, "%c ", isalnum(buf[i] & 0xff) ? buf[i] : '.');
		}

	}

	if (label) {
		fprintf(stderr, "\n--------------\n");
	}
}

int if_br_port_num(const char *ifname)
{
	char path[512] = {0};
	int portnum;
	FILE *f;

	snprintf(path, 512, "/sys/class/net/%s/brport/port_no", ifname);
	f = fopen(path, "r"); /* cppcheck-suppress cert-MSC24-C */

	if (!f) {
		return -1;
	}

	if (fscanf(f, "%i", &portnum) != 1) {
		fclose(f);
		return -1;
	}

	fclose(f);

	return portnum;
}

int if_get_carrier(const char *ifname, int *carrier)
{
	char path[256] = {0};
	char buf[32] = {0};
	int ret;
	int fd;
	char *endptr = NULL;

	*carrier = -1;
	snprintf(path, sizeof(path), "/sys/class/net/%s/carrier", ifname);
	fd = open(path, O_RDONLY);

	if (fd < 0) {
		return -1;
	}

	ret = read(fd, buf, sizeof(buf));
	close(fd);

	if (ret <= 0) {
		return -1;
	}

	errno = 0;
	*carrier = strtol(buf, &endptr, 10);
	
	if (errno) {
		fprintf(stderr, "Invalid carrier value: %s\n", buf);
		return -1;
	}

	return 0;
}

const char *regex_match(const char *str, const char *pattern)
{
	const char *s = str;
	regmatch_t pmatch[1];
	regex_t regex;
	int status;

	if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED)) {
		return NULL;
	}

	status = regexec(&regex, s, 1, pmatch, 0);
	regfree(&regex);

	if (status != 0) {
		return NULL;
	}

	return s + pmatch[0].rm_so;
}

char *strstr_exact(char *haystack, const char *needle)
{
	char *s, *tmp;
	char *str;

	if (!haystack || !needle) {
		return NULL;
	}

	str = strdup(haystack);

	foreach_token_r(s, str, tmp, ", ") {
		if (!strncmp(s, needle, max(strlen(s), strlen(needle)))) {
			break;
		}
	}

	free(str);
	
	return s;
}

uint8_t *strtob(char *str, int len, uint8_t *bytes)
{
	size_t slen;
	int i;

	if (!str || !bytes)
		return NULL;

	slen = strlen(str);
	if (!slen || slen % 2 || str[slen] != '\0')
		return NULL;

	slen >>= 1;
	if (len > (int)slen)
		len = (int)slen;

	for (i = 0; i < len; i++) {
		int a;

		if ((a = hex2byte(str)) < 0)
			return NULL;

		str += 2;
		bytes[i] = (uint8_t)a;
	}

	return bytes;
}

#endif 