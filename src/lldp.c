/*
 * lldp.c - Link Layer Discovery Protocol function.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include <stdlib.h>
#include <string.h>

#include "lldp.h"
#include "ieee_1905_tlv.h"
#include "parameters.h"
#include "cmdu.h"
#include "buff_util.h"

struct cmdu_buff * 
build_ieee_1905_bridge_discovery(
	struct ieee_1905_interface * ieee_1905_if)
{
	struct cmdu_buff *frm = NULL;
	uint8_t *lldp_buf;
	int ret = 0;

	frm = cmdu_alloc_default();

	if (!frm) {
		return NULL;
	}

	frm->data = (uint8_t *)(frm->head + 18);
	frm->tail = frm->data;

	lldp_buf = calloc(1, 256* sizeof(uint8_t));

	if (!lldp_buf) {
		cmdu_free(frm);
		return NULL;
	}

	lldp_buf[0] = (LLDP_TLV_CHASSIS_ID << 1) | ((7 & 0x80) >> 7);
	lldp_buf[1] = 7 & 0x7f;
	lldp_buf[2] = LLDP_CHASSIS_ID_SUBTYPE_MAC_ADDRESS;
	(void)memcpy(&lldp_buf[3], ieee_1905_if->al_addr, MAC_LEN);

	ret = cmdu_put(frm, lldp_buf, 9);

	if (ret) {
		goto error;
	}

	(void)memset(lldp_buf, 0, 256);
	lldp_buf[0] = (LLDP_TLV_TTL << 1) | ((2 & 0x80) >> 7);
	lldp_buf[1] = 2 & 0x7f;
	buf_put_be16(&lldp_buf[2], LLDP_TTL_1905_DEFAULT_VALUE);

	ret = cmdu_put(frm, lldp_buf, 4);

	if (!ret) {
		goto error;
	}

	return frm;

error:
	cmdu_free(frm);
	free(lldp_buf);
	return NULL;
}