/*
 * ieee_1905_cmdu.h: cmdu message definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _IEEE_1905_CMDU_H_
#define _IEEE_1905_CMDU_H_

#include <stdint.h>

#include "cmdu.h"
#include "ieee_1905_data_model.h"

/*****************************************************************************************************
 * 									MACRO DECLEARATION												 *
 *****************************************************************************************************/

/*****************************************************************************************************
 * 							      STRUCTURE DECLEARATION											 *
 *****************************************************************************************************/

/*****************************************************************************************************
 * 								      FUNCTION DECLEARATION											 *
 *****************************************************************************************************/

struct cmdu_buff *
build_ieee1905_topology_discovery(
	struct ieee_1905_interface * ieee_1905_if,
	uint16_t mid);

struct cmdu_buff *
build_ieee1905_topology_query(
	uint16_t mid);

struct cmdu_buff *
build_ieee1905_link_metric_query(
	uint16_t mid);

struct cmdu_buff *
build_ieee1905_topology_notification(
	struct ieee_1905_interface * ieee_1905_if,
	uint16_t mid);

struct cmdu_buff *
build_ieee1905_ap_autoconfig_search(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t mid,
	uint8_t freqband);

struct cmdu_buff *
build_ieee1905_ap_autoconfig_renew(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t mid,
	uint8_t freqband);

struct cmdu_buff *
build_ieee1905_ap_autoconfig_response(
	uint16_t mid,
	uint8_t freqband);

struct cmdu_buff *
build_ieee1905_vendor_specific(
	uint16_t mid,
	int argc, 
	char *argv[]);

struct cmdu_buff *
build_cmdu_message_to_send(
	struct ieee_1905_interface *ieee_1905_if,
	uint16_t vid,
	uint16_t mid,
	uint8_t *dst,
	uint8_t *src,
	uint16_t eth_type,
	struct cmdu_buff *frm);

#endif /* END _IEEE_1905_CMDU_H */