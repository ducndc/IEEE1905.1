#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdu.h"
#include "utils.h"

int main(void) 
{
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

    return 0;
}