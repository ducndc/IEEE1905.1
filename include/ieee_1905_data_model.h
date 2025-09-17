/*
 * ieee_1905_data_model.h: cmdu message definition in flat format.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#ifndef _IEEE_1905_DATA_MODEL_H_
#define _IEEE_1905_DATA_MODEL_H_

#include "parameters.h"
#include "ieee_1905_tlv.h"
#include "utils.h"
#include "timer.h"

/*****************************************************************************************************
 * 									MACRO DECLEARATION												 *
 *****************************************************************************************************/

/*****************************************************************************************************
 * 							      STRUCTURE DECLEARATION											 *
 *****************************************************************************************************/

enum ieee_1905_media_type {
	I1905_802_3U_FAST_ETHERNET       = (0x0000),
	I1905_802_3AB_GIGABIT_ETHERNET   = (0x0001),
	I1905_802_11B_2_4_GHZ            = (0x0100),
	I1905_802_11G_2_4_GHZ            = (0x0101),
	I1905_802_11A_5_GHZ              = (0x0102),
	I1905_802_11N_2_4_GHZ            = (0x0103),
	I1905_802_11N_5_GHZ              = (0x0104),
	I1905_802_11AC_5_GHZ             = (0x0105),
	I1905_802_11AD_60_GHZ            = (0x0106),
	I1905_802_11AF_GHZ               = (0x0107),
	I1905_802_11AX                   = (0x0108),  /* 11ax */
	I1905_802_11BE                   = (0x0109),  /* 11be */
	I1905_1901_WAVELET               = (0x0200),
	I1905_1901_FFT                   = (0x0201),
	I1905_MOCA_V1_1                  = (0x0300),
	I1905_MEDIA_UNKNOWN              = (0xFFFF),
};

enum ieee_1905_version {
	I1905_VERSION_DOT_1,
	I1905_VERSION_DOT_1A,
	I1905_VERSION_INVALID,
};

enum ieee_1905_security_method {
	I1905_SECURITY_UNKNOWN,
	I1905_SECURITY_UCPK,
	I1905_SECURITY_PBC,
	I1905_SECURITY_NFC,
	I1905_SECURITY_INVALID,
};

enum ieee_1905_if_power_state {
	I1905_IFPOWER_ON,
	I1905_IFPOWER_PS,
	I1905_IFPOWER_OFF,
	I1905_IFPOWER_NOTSUPP,
	I1905_IFPOWER_INVALID,
};

struct ieee_1905_genphy {
	uint8_t oui[OUI_LEN];
	uint8_t variant;
	char *url;
};

struct ieee_1905_non1905_neighbor {
	uint8_t mac_addr[MAC_LEN];
	struct list_head list;
};

struct ieee_1905_vendor_info {
	uint8_t oui[OUI_LEN];
	uint8_t *data;
	struct list_head list;
};

struct ieee_1905_fwd_table {
	bool allow;
	struct list_head rule_list;
};

struct ieee_1905_topology {
	bool enable;
	uint8_t status;			
	uint32_t num_devices;		
	struct list_head devlist;
};

struct ieee_1905_security {
	enum ieee_1905_security_method method;
	uint8_t password[64];
};

struct ieee_1905_metric {
	bool br_present;
	uint32_t tx_errors;
	uint32_t rx_errors;
	uint32_t tx_packets;
	uint32_t rx_packets;
	uint32_t available;
	uint32_t max_rate;
	uint32_t max_phyrate;
	uint8_t rcpi;
};

struct ieee_1905_wifi_media_info {
	uint8_t band;
	enum ieee_1905_media_type media;
	struct ieee_80211_info info;
};

struct ieee_1905_interface {
    /* 64-bit pointers & large objects first */
    void *device;
    void *priv;
    struct ip_address *ip_addrs;

    /* list heads (2 pointers each) */
    struct list_head vendor_list;
    struct list_head non1905_nbr_list;
    struct list_head nbr_if_list;
    struct list_head list;

    /* fixed 32-bit values */
    uint32_t if_index;
    uint32_t if_status;
    uint32_t if_flags;
    uint32_t br_port;
    uint32_t br_if_index;
    uint32_t num_vendor;
    uint32_t num_ip_addrs;
    uint32_t num_links;
    uint32_t num_neighbor_non1905;

    /* enums (usually 4 bytes each) */
    enum if_media_type media_type;
    enum ieee_1905_media_type media;
    enum ieee_1905_if_power_state power;

    /* 16-bit */
    uint16_t vid;

    /* 8-bit arrays and small integers */
    uint8_t mac_addr[MAC_LEN];
    uint8_t al_addr[MAC_LEN];
    uint8_t operation_state;

    /* bitfields for bool to save space */
    unsigned int authenticated   : 1;
    unsigned int pbc_supported   : 1;
    unsigned int pbc_ongoing     : 1;
    unsigned int is_registrar    : 1;
    unsigned int upstream        : 1;
    unsigned int no_tag          : 1;
    unsigned int exclude         : 1;
    unsigned int lo              : 1;
    unsigned int invalid         : 1;
    unsigned int is_brif         : 1;
    unsigned int allow_if_power  : 1;

    /* other small integers */
    int no_master;
    int carrier;
    int num_media_info;

    /* nested struct */
    struct ieee_1905_genphy genphy;

    /* pointer last */
    uint8_t *media_info;

    /* char array last (depends on DEV_NAME_LEN) */
    char if_name[DEV_NAME_LEN];
};

struct ieee_1905_neighbor_interface {
	uint8_t mac_addr[MAC_LEN];
	uint8_t al_addr[MAC_LEN];
	bool has_bridge;
	enum ieee_1905_media_type media;
	struct ieee_1905_genphy genphy;
	struct ieee_1905_metric metric;
	struct list_head list;
	bool direct;
	atimer_t stale_timer;
	struct ieee_1905_interface *iface;
	bool invalid;
};

struct ieee_1905_own_device_interface {
	char if_name[DEV_NAME_LEN];
	uint8_t mac_addr[MAC_LEN];
	int exclude;
	bool is_bridge;
	uint32_t if_status;
	uint16_t media_type;	
	uint8_t media_info[MEDIA_INFO_LEN];	
	struct list_head list;
};

struct ieee_1905_master_interface {
	char if_name[DEV_NAME_LEN];
	uint32_t ifindex;
	uint8_t mac_addr[MAC_LEN];
	int exclude;
	uint32_t ifstatus;
	uint32_t num_ip_addrs;
	struct ip_address *ip_addrs;
	struct list_head list;
};


struct ieee_1905_own_device {
	uint32_t tsp;
	bool enabled;
	uint8_t al_addr[MAC_LEN];
	enum ieee_1905_version version;
	uint8_t reg_band;
	char name[NAME_LEN];
	char manufacturer[NAME_LEN];
	char model[NAME_LEN];
	char *url;
	uint32_t num_interface;
	struct list_head if_list;
	int num_local_interface;
	struct list_head local_if_list;
	uint32_t num_master_interface;
	struct list_head master_if_list;
	int num_non1905_neighbor;
	uint8_t frag_scheme;
	struct ieee_1905_fwd_table fwd;
	struct ieee_1905_topology topology;
	struct ieee_1905_security security;
	uint8_t net_registar[3][MAC_LEN];
};

struct ieee_1905_data_model {
	struct ieee_1905_own_device self;
};

/*****************************************************************************************************
 * 								      FUNCTION DECLEARATION											 *
 *****************************************************************************************************/

#endif /* END _IEEE_1905_DATA_MODEL_H_ */