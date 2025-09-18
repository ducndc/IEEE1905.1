/*
 * ieee_1905_cmdu.h: cmdu message definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include <string.h>
#include <net/ethernet.h>

#include "cmdu.h"
#include "ieee_1905_tlv.h"
#include "parameters.h"
#include "ieee_1905_data_model.h"
#include "utils.h"
#include "buff_util.h"

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
build_ieee1905_topology_notification(
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

struct cmdu_buff *
build_ieee1905_ap_autoconfig_search(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t mid,
	uint8_t freqband)
{
	struct cmdu_buff *frm = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	frm = cmdu_alloc_simple(CMDU_TYPE_AP_AUTOCONFIGURATION_SEARCH, &mid);

	if (!frm) {
		return NULL;
	}

	CMDU_SET_RELAY_MCAST(frm->cdata);

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

	t = cmdu_reserve_tlv(frm, 256);
	
	if (!t) {
		cmdu_free(frm);
		return NULL;
	}	

	t->type = TLV_TYPE_SEARCHED_ROLE;
	t->len = 1;
	t->data[0] = 0x00;
	ret = cmdu_put_tlv(frm, t);
	
	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

		t = cmdu_reserve_tlv(frm, 256);
	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_AUTOCONFIG_FREQ_BAND;
	t->len = 1;
	t->data[0] = freqband;

	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	cmdu_put_eom(frm);

	return frm;
}

struct cmdu_buff *
build_ieee1905_ap_autoconfig_renew(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t mid,
	uint8_t freqband)
{
	struct cmdu_buff *frm = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	frm = cmdu_alloc_simple(CMDU_TYPE_AP_AUTOCONFIGURATION_RENEW, &mid);

	if (!frm) {
		return NULL;
	}

	CMDU_SET_RELAY_MCAST(frm->cdata);
	t = cmdu_reserve_tlv(frm, 256);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_AL_MAC_ADDRESS_TYPE;
	t->len = 6;
	memcpy(t->data, ieee_1905_if->al_addr, MAC_LEN);

	ret = cmdu_put_tlv(frm, t);
	
	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, 256);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_SUPPORTED_ROLE;
	t->len = 1;
	t->data[0] = 0x00;
	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	t = cmdu_reserve_tlv(frm, 256);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_SUPPORTED_FREQ_BAND;
	t->len = 1;
	t->data[0] = freqband;

	ret = cmdu_put_tlv(frm, t);

	if (ret) {
		cmdu_free(frm);
		return NULL;
	}

	cmdu_put_eom(frm);
	
	return frm;
}

struct cmdu_buff *
build_ieee1905_ap_autoconfig_response(
	uint16_t mid,
	uint8_t freqband)
{
	struct cmdu_buff *resp = NULL;
	struct tlv *t = NULL;
	int ret = 0;

	resp = cmdu_alloc_simple(CMDU_TYPE_AP_AUTOCONFIGURATION_RESPONSE, &mid);

	if (!resp) {
		return NULL;
	}

	t = cmdu_reserve_tlv(resp, 256);

	if (!t) {
		cmdu_free(resp);
		return NULL;
	}

	t->type = TLV_TYPE_SUPPORTED_ROLE;
	t->len = 1;
	t->data[0] = 0x00;
	ret = cmdu_put_tlv(resp, t);

	if (ret) {
		cmdu_free(resp);
		return NULL;
	}

	t = cmdu_reserve_tlv(resp, 256);

	if (!t) {
		cmdu_free(resp);
		return NULL;
	}

	t->type = TLV_TYPE_SUPPORTED_FREQ_BAND;
	t->len = 1;
	t->data[0] = freqband;
	ret = cmdu_put_tlv(resp, t);

	if (ret) {
		cmdu_free(resp);
		return NULL;
	}

	cmdu_put_eom(resp);

	return resp;
}

struct cmdu_buff *
build_ieee1905_vendor_specific(
	uint16_t mid,
	int argc, 
	char *argv[])
{
	struct tlv_vendor_specific *vs;
	struct cmdu_buff *frm = NULL;
	size_t datalen = 0;
	struct tlv *t = NULL;
	int ret = 0;

	if (argc > 2) {
		return NULL;
	}

	if (argv[0] && strlen(argv[0]) != 6) {
		return NULL;
	}

	if (argv[1]) {
		datalen = strlen(argv[1]) / 2;
	}

	frm = cmdu_alloc_simple(CMDU_TYPE_VENDOR_SPECIFIC, &mid);

	if (!frm) {
		return NULL;
	}

	/* prepare TLVs */
	t = cmdu_reserve_tlv(frm, 3 + datalen);

	if (!t) {
		cmdu_free(frm);
		return NULL;
	}

	t->type = TLV_TYPE_VENDOR_SPECIFIC;
	t->len = 3;
	vs = (struct tlv_vendor_specific *)t->data;

	if (argv[0]) {
		strtob(argv[0], 3, vs->oui);
	}

	if (argv[1]) {
		strtob(argv[1], datalen, vs->bytes);
		t->len += datalen;
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
build_cmdu_message_to_send(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t vid,
	uint16_t mid,
	uint8_t *dst,
	uint8_t *src,
	uint16_t eth_type,
	struct cmdu_buff *frm)
{
	struct ether_header *eth;

	if (!frm) {
		return NULL;
	}

	if (ieee_1905_if->no_tag) {
		BUF_PUT_BE16(vid, 0);
	}

	if (frm->cdata && (frm->cdata->hdr.mid == 0)) {
		frm->cdata->hdr.mid = mid;
	}

	frm->len = frm->data_len + sizeof(struct ether_header) + sizeof(struct cmdu_header);

	if (vid > 0) {
		frm->len += 4;
	} else {
		frm->head += 4;
	}

	eth = (struct ether_header *)frm->head;

	if (hwaddr_is_zero(src)) {
		(void)memcpy(eth->ether_shost, ieee_1905_if->al_addr, MAC_LEN);
	} else {
		(void)memcpy(eth->ether_shost, src, MAC_LEN);
	}

	if (hwaddr_is_zero(dst)) {
		(void)memcpy(eth->ether_dhost, ieee_1905_if->al_addr, MAC_LEN);
	} else {
		(void)memcpy(eth->ether_dhost, dst, MAC_LEN);
	}

	eth->ether_type = vid > 0 ? htons(0x8100) : htons(eth_type);

	if (vid > 0) {
		buf_put_be16(frm->head + ETH_HLEN, vid);
		buf_put_be16(frm->head + ETH_HLEN + 2, eth_type);
	}

	return frm;
}