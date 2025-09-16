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

/*****************************************************************************************************
 * 									MACRO DECLEARATION												 *
 *****************************************************************************************************/

/*****************************************************************************************************
 * 							      STRUCTURE DECLEARATION											 *
 *****************************************************************************************************/

struct ieee_1905_interface {
	char if_name[DEV_NAME_LEN];		/**< interface name */
	uint32_t if_index;				/**< interface index; valid only it belongs to i1905_selfdevice */
	uint8_t mac_addr[MAC_LEN];		/**< macaddress of the interface */
	uint8_t al_addr[MAC_LEN];		/**< 1905 ALID or AL-macaddress or the device this interface belongs to */
	uint32_t ifstatus;				/**< interface status */
};

/*****************************************************************************************************
 * 								      FUNCTION DECLEARATION											 *
 *****************************************************************************************************/

#endif /* END _IEEE_1905_DATA_MODEL_H_ */