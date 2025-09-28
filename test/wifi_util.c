/*
 * wifi_util.c: Wi-Fi util.
 *
 * Copyright (C) 2025
 *
 * Author: Chung Duc Nguyen Dang
 */

#include "wifi_util.h"

uint8_t
rssi_to_rcpi(
	int rssi)
{
	if (!rssi) {
		return 255;
	}

	if (rssi < -110) {
		return 0;
	}

	if (rssi > 0) {
		return 220;
	}

	return ((rssi + 110) * 2);
}

int 
rcpi_to_rssi(
	uint8_t rcpi)
{
	if (rcpi == 0) {
		return -110;
	}

	if (rcpi == 255) {
		return 0;
	}

	if (rcpi > 220) {
		return 0;
	}

	return ((rcpi / 2) - 110);
}