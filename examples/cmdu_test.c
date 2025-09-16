#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdu.h"
#include "utils.h"
#include "ieee_1905_cmdu.h"
#include "ieee_1905_data_model.h"
#include "buff_util.h"

#define IEEE_1905_DATA_MODEL 1

int main(void) 
{

#ifndef IEEE_1905_DATA_MODEL
    struct cmdu_buff *c;
   

    c = cmdu_alloc_default();

    if (!c) {
        printf("Failed to allocate CMDU buffer\n");
        return 1;
    }

    (void)memcpy(c->dev_ifname, "rax3", 4);
    
    // Print buffer size
    printf("Allocated CMDU buffer of size:\t%d bytes\n", cmdu_size(c));
    printf("MAC address:\t\t\t" MACFMT "\n", MAC2STR(c->dev_macaddr));
    printf("Interface name:\t\t\t%s\n", c->dev_ifname);
    printf("Origin MAC:\t\t\t" MACFMT "\n", MAC2STR(c->origin));
    printf("AL MAC:\t\t\t\t" MACFMT "\n", MAC2STR(c->aladdr));
    printf("Flag:\t\t\t\t%d \n", c->flags);
    printf("Data length:\t\t\t%d bytes \n", c->datalen);
    printf("Number of frags:\t\t%d \n", c->num_frags);

    cmdu_free(c);

#else
    struct cmdu_buff *c;
    struct ieee_1905_interface *interface;
    struct tlv *t;
    uint16_t mid = 99;

    interface = (struct ieee_1905_interface *)malloc(sizeof(struct ieee_1905_interface));

    if (!interface) {
        printf("Failed to allocate ieee 1905 interface\n");
        return -1;
    }

    (void)memset(interface, 0, sizeof(struct ieee_1905_interface));
    (void)memcpy(interface->if_name, "rax3", 4);
    interface->if_index = 4;
    (void)memcpy(interface->mac_addr, "123456", 6);
    (void)memcpy(interface->al_addr, "123456", 6);
    interface->ifstatus = 0;

    c = build_ieee1905_topology_discovery(interface, mid);

    if (!c) {
        printf("Failed to create ieee 1905 topology discovery\n");
        free(interface);
        return 1;
    }

    (void)memcpy(c->dev_ifname, "rax3", 4);

    printf("Allocated CMDU buffer of size:\t%d bytes\n", cmdu_size(c));
    printf("MAC address:\t\t\t" MACFMT "\n", MAC2STR(c->dev_macaddr));
    printf("Interface name:\t\t\t%s\n", c->dev_ifname);
    printf("Origin MAC:\t\t\t" MACFMT "\n", MAC2STR(c->origin));
    printf("AL MAC:\t\t\t\t" MACFMT "\n", MAC2STR(c->aladdr));
    printf("Flag:\t\t\t\t%d \n", c->flags);
    printf("Data length:\t\t\t%d bytes \n", c->datalen);
    printf("Number of frags:\t\t%d \n", c->num_frags);
    printf("MID:\t\t\t\t%d\n", cmdu_get_mid(c));

    t = cmdu_reserve_tlv(c, MAC_LEN);
    printf("\nTLV Information\n");
    printf("Type:\t\t\t\t%d\n", t->type);
    printf("Length:\t\t\t\t%d\n", t->len);
    printf("Data:\t\t\t\t\t%s\n", t->data);

    cmdu_free(c);

#endif

    return 0;
}