#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip-8.h"


#define START_ADD 0x200 /* Where programs start in RAM */
#define TOTAL_RAM 4096
#define RAM_LIMIT 0xE8F /* RAM limit for user programs */

uint8_t chip_rand()
{
    return rand() % 256;
}

chp8_t * init_chip() {
  printf("rand initialized: %d\n", chip_rand());
  chp8_t  *c8 = malloc(sizeof(chp8_t));
  memset(c8, 0, sizeof(chp8_t));
  printf("chip initialized\n");
  return c8;
}
