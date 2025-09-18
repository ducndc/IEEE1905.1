#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>

#include "cmdu.h"
#include "utils.h"
#include "ieee_1905_cmdu.h"
#include "ieee_1905_data_model.h"
#include "buff_util.h"
#include "ieee_1905_tlv.h"

#define TOPOLOGY_DISCOVERY

void 
dump_buffer(
    const uint8_t *buf, 
    size_t len) 
{
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
        if ((i + 1) % 16 == 0) // ngắt dòng mỗi 16 byte
            printf("\n");
    }
    if (len % 16 != 0)
        printf("\n");
}

int main(void) 
{
    struct cmdu_buff *c;
    struct ieee_1905_interface *interface;
    struct tlv *t;
    struct ether_header *eth;
    uint16_t mid = 99;
    uint8_t mac[6] = {0x24, 0x0B, 0x2A, 0xFF, 0xE9, 0x64};
    uint8_t al_mac[6] = {0x26, 0x0B, 0x2A, 0xFF, 0xE9, 0x65};

    interface = (struct ieee_1905_interface *)malloc(sizeof(struct ieee_1905_interface));

    if (!interface) {
        printf("Failed to allocate ieee 1905 interface\n");
        return -1;
    }

    (void)memset(interface, 0, sizeof(struct ieee_1905_interface));
    (void)memcpy(interface->if_name, "rax3", 4);
    interface->if_index = 4;
    (void)memcpy(interface->mac_addr, mac, MAC_LEN);
    (void)memcpy(interface->al_addr, al_mac, MAC_LEN);
    interface->if_status = 0;

#ifdef TOPOLOGY_DISCOVERY
    c = build_ieee1905_topology_discovery(interface, mid);
#else 
    c = build_ieee1905_ap_autoconfig_search(interface, mid, 2);
#endif

    if (!c) {
        printf("Failed to create ieee 1905 topology discovery\n");
        free(interface);
        return 1;
    }

    struct cmdu_buff *send_message = NULL;
    (void)memcpy(c->dev_if_name, "rax3", 4);

    send_message = build_cmdu_message_to_send(interface, 9, 10, mac, al_mac, 10, c);
    eth = (struct ether_header *)send_message->head;

    printf("Allocated CMDU buffer of size:\t%d bytes\n", cmdu_size(send_message));
    printf("Source MAC address:\t\t" MACFMT "\n", MAC2STR(eth->ether_shost));
    printf("Destination MAC address:\t" MACFMT "\n", MAC2STR(eth->ether_dhost));
    printf("Interface name:\t\t\t%s\n", send_message->dev_if_name);
    printf("Flag:\t\t\t\t%d \n", send_message->flags);
    printf("Data length:\t\t\t%d bytes \n", send_message->data_len);
    printf("Number of frags:\t\t%d \n", send_message->num_frags);
    printf("MID:\t\t\t\t%d\n", cmdu_get_mid(send_message));

    //dump_buffer((uint8_t *)send_message, send_message->data_len);

#ifdef TOPOLOGY_DISCOVERY
    t = cmdu_extract_tlv(send_message, TLV_TYPE_AL_MAC_ADDRESS_TYPE);

    if (!t) {
        goto error;
    }

    printf("\nTLV Information\n");
    printf("Type:\t\t\t\t%s\n", tlv_type2str(t->type));
    printf("Length:\t\t\t\t%d\n", tlv_total_length(t));
    printf("Data:\t\t\t\t"MACFMT"\n", MAC2STR(t->data));

    t = cmdu_extract_tlv(send_message, TLV_TYPE_MAC_ADDRESS_TYPE);

    if (!t) {
        goto error;
    }

    printf("\nTLV Information\n");
    printf("Type:\t\t\t\t%s\n", tlv_type2str(t->type));
    printf("Length:\t\t\t\t%d\n", tlv_total_length(t));
    printf("Data:\t\t\t\t"MACFMT"\n", MAC2STR(t->data));
#else
    t = cmdu_extract_tlv(send_message, TLV_TYPE_AL_MAC_ADDRESS_TYPE);

    if (!t) {
        goto error;
    }

    printf("\nTLV Information\n");
    printf("Type:\t\t\t\t%s\n", tlv_type2str(t->type));
    printf("Length:\t\t\t\t%d\n", tlv_total_length(t));
    printf("Data:\t\t\t\t"MACFMT"\n", MAC2STR(t->data));

    t = cmdu_extract_tlv(c, TLV_TYPE_SEARCHED_ROLE);

    if (!t) {
        goto error;
    }

    printf("\nTLV Information\n");
    printf("Type:\t\t\t\t%s\n", tlv_type2str(t->type));
    printf("Length:\t\t\t\t%d\n", tlv_total_length(t));
    printf("Data:\t\t\t\t"MACFMT"\n", MAC2STR(t->data));
#endif

error:
    cmdu_free(c);

    return 0;
}