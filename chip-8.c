#include <stdint.h>
#include <stdio.h>

#define START_ADD 0x200 /* Where programs start in RAM */
#define TOTAL_RAM 4096
#define RAM_LIMIT 0xE8F /* RAM limit for user programs */

void init_chip() {
  uint8_t v[16];
  uint8_t ram[TOTAL_RAM];
  uint16_t i_register;
  uint16_t pc; /* Program counter */
  uint16_t sp; /* Stack pointer */
  /* TODO: add SDL delay timer & sound timer */

  printf("chip initialized\n");
}
