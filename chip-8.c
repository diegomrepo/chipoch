#include "chip-8.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t chip_rand() { return rand() % 256; }

chp8_t *init_chip() {
    printf("rand initialized: %d\n", chip_rand());
    chp8_t *c8 = malloc(sizeof(chp8_t));
    memset(c8, 0, sizeof(chp8_t));
    memset(c8->memory, 0, MEM_SIZE);         /* best way? */
    memset(c8->video, 0, sizeof(c8->video)); /* best way? */
    memset(c8->V, 0, sizeof(c8->V)); /* TODO: Find out if this is best way */
    c8->i_register = 0;
    c8->pc = START_ADDR;
    c8->sp = 0;

    printf("chip initialized\n");
    return c8;
}
