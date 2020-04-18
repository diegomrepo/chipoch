#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "chip-8.h"
#include "video.h"

void cycle(chp8_t *c8, bool *event) {
    // dump_memory(c8);
    uint16_t opcode = 0;

    printf("PC = 0x%04x\n", c8->pc);
    opcode =
        c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1]; // get instruction
    c8->pc = c8->pc + 2;
    printf("opcode = 0x%04x\n", opcode);
    if (opcode == 0)
        *event = true;

    int num = chip_rand();
    /* putchar('\n'); printf("%d", num); putchar('\n');*/
    /* if (num % 255 == 0) *event = true; SDL_Delay(100); */
    execute(opcode);
    if (c8->pc > RAM_LIMIT)
        *event = true;
}

uint8_t chip_rand() { return rand() % 256; }
void op0NNN(uint16_t op) {
     printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op00E0(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
    printf("** CLS **");
    clr(renderer, BLACK);
}
void op00EE(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
    //STACK
}
void op1NNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op2XNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op3XNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op4XNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op5XY0(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op6XNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op7XNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
/* multi 8 */
void op8XN0(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN1(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN2(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN3(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN4(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN5(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN6(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XN7(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XNE(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void op8XYN(uint16_t op) {
    uint8_t nibble = op & 0x000f;
    void (*op8YN_fn[0x10])(uint16_t op) = {
        op8XN0, op8XN1, op8XN2, op8XN3, op8XN4, op8XN5, op8XN6, op8XN7,
        NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   op8XNE, NULL};
    printf("MULTI-OPCODE: %s, %04x\n", __func__, op);
    op8YN_fn[nibble](op);
}

void op9XY0(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opANNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opBNNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opCXNN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opDXYN(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opEX9E(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opEXA1(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
/* multi F */
void opFX07(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX0A(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX15(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX18(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX1E(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX29(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX33(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX55(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFX65(uint16_t op) {
    printf("Opcode function is: %s with %04x\n", __func__, op);
}
void opFXNN(uint16_t op) {

    uint8_t byte = op & 0x00ff;
    printf("MULTI-OPCODE: %s, %04x\n", __func__, op);
    void (*opFX_fn[0x100])(uint16_t op) = {NULL};
    opFX_fn[0x07] = opFX07;
    opFX_fn[0x0A] = opFX0A;
    opFX_fn[0x15] = opFX15;
    opFX_fn[0x18] = opFX15;
    opFX_fn[0x29] = opFX29;
    opFX_fn[0x33] = opFX33;
    opFX_fn[0x55] = opFX55;
    opFX_fn[0x65] = opFX65;
    opFX_fn[byte](op);
}

void (*cmd_pointer[OPCODE_MAX])(uint16_t opcode) = {NULL};
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

    /* typedef struct {
        void (*op_fn)(void);
        uint16_t op_name;
    } cmd_pr;
    cmd_pr *cmd_pointer[0x10000];
    */

    /* Can we optimize with a while or a custom for? */
    for (int i = 0x0000; i < 0x1000; i++)
        cmd_pointer[i] = op0NNN;
    for (int i = 0x1000; i < 0x2000; i++)
        cmd_pointer[i] = op1NNN;
    for (int i = 0x2000; i < 0x3000; i++)
        cmd_pointer[i] = op2XNN;
    for (int i = 0x3000; i < 0x4000; i++)
        cmd_pointer[i] = op3XNN;
    for (int i = 0x4000; i < 0x5000; i++)
        cmd_pointer[i] = op4XNN;
    for (int i = 0x5000; i < 0x6000; i++)
        cmd_pointer[i] = op5XY0;
    for (int i = 0x6000; i < 0x7000; i++)
        cmd_pointer[i] = op6XNN;
    for (int i = 0x7000; i < 0x8000; i++)
        cmd_pointer[i] = op7XNN;
    for (int i = 0x8000; i < 0x9000; i++)
        cmd_pointer[i] = op8XYN;
    for (int i = 0x9000; i < 0xA000; i++)
        cmd_pointer[i] = op9XY0;
    for (int i = 0xA000; i < 0xB000; i++)
        cmd_pointer[i] = opANNN;
    for (int i = 0xB000; i < 0xC000; i++)
        cmd_pointer[i] = opBNNN;
    for (int i = 0xC000; i < 0xD000; i++)
        cmd_pointer[i] = opCXNN;
    for (int i = 0xD000; i < 0xE000; i++)
        cmd_pointer[i] = opDXYN;
    for (int i = 0xE000; i < 0xF000; i++)
        cmd_pointer[i] = opEX9E;
    for (int i = 0xF000; i < OPCODE_MAX; i++)
        cmd_pointer[i] = opFXNN;
    cmd_pointer[0x00E0] = op00E0;
    cmd_pointer[0x00EE] = op00EE;

    printf("chip initialized\n");
    return c8;
}
void execute(uint16_t op) {
    if (cmd_pointer[op])
        cmd_pointer[op](op);
    else
        printf("%04x not implmenent/undefined\n", op);
}
