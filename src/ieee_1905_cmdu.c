/*
 * ieee_1905_cmdu.h: cmdu message definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include <string.h>

#include "cmdu.h"
#include "ieee_1905_tlv.h"
#include "parameters.h"
#include "ieee_1905_data_model.h"
#include "utils.h"

struct cmdu_buff *
build_ieee1905_topology_discovery(
	struct ieee_1905_interface * ieee_1905_if, 
	uint16_t mid)
{
	struct cmdu_buff *frm = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	frm = cmdu_alloc_simple(CMDU_TYPE_TOPOLOGY_DISCOVERY, &mid);

	if (!frm) {
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, MAC_LEN);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_AL_MAC_ADDRESS_TYPE;
	t->len = MAC_LEN;

	(void)memcpy(t->data, ieee_1905_if->al_addr, MAC_LEN);
	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, MAC_LEN);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_MAC_ADDRESS_TYPE;
	t->len = MAC_LEN;

	if (!hwaddr_is_zero(ieee_1905_if->mac_addr)) {
		memcpy(t->data, ieee_1905_if->mac_addr, MAC_LEN);
	} else {
		memcpy(t->data, ieee_1905_if->al_addr, MAC_LEN);
	}

	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	cmdu_put_eom(frm);

	return frm;
}

struct cmdu_buff *
build_ieee1905_topology_query(
	uint16_t mid)
{
	struct cmdu_buff *frm = NULL;

	frm = cmdu_alloc_simple(CMDU_TYPE_TOPOLOGY_QUERY, &mid);

	if (!frm) {
		return NULL;
	}

	return frm;
}

struct cmdu_buff *
build_ieee1905_link_metric_query(
	uint16_t mid)
{
	struct tlv_link_metric_query *lq;
	struct cmdu_buff *frm = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	frm = cmdu_alloc_simple(CMDU_TYPE_LINK_METRIC_QUERY, &mid);
	
	if (!frm) {
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, 256);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_LINK_METRIC_QUERY;
	t->len = sizeof(struct tlv_link_metric_query);
	lq = (struct tlv_link_metric_query *)t->data;
	lq->nbr_type = LINK_METRIC_QUERY_NEIGHBOR_ALL;
	lq->query_type = LINK_METRIC_QUERY_TYPE_BOTH;

	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	cmdu_put_eom(frm);

	return frm;
}

struct cmdu_buff *
i1905_build_topology_notification(
	struct ieee_1905_interface * ieee_1905_if,
	uint16_t mid)
{
	struct cmdu_buff *frm = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	frm = cmdu_alloc_simple(CMDU_TYPE_TOPOLOGY_NOTIFICATION, &mid);

	if (!frm) {
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, 256);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_AL_MAC_ADDRESS_TYPE;
	t->len = MAC_LEN;
	memcpy(t->data, ieee_1905_if->al_addr, MAC_LEN);

	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	cmdu_put_eom(frm);

	return frm;
}