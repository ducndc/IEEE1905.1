#include <stdio.h>
#include <stdlib.h>

#include "cmdu.h"

int main(void) {
    struct cmdu_buff *c;

    // Allocate a default CMDU buffer
    c = cmdu_alloc_default();
    if (!c) {
        printf("Failed to allocate CMDU buffer\n");
        return 1;
    }

    // Print buffer size
    printf("Allocated CMDU buffer of size: %d bytes\n", cmdu_size(c));

    // Free the buffer
    cmdu_free(c);

    return 0;
}