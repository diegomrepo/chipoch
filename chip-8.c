#include "chip-8.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t chip_rand() { return rand() % 256; }
void op0NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op00E0(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op00EE(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op1NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op2NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op3NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op4NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op5NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op6NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op7NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op8NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void op9NNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opANNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opBNNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opCNNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opDNNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opENNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}
void opFNNN(uint16_t op) {printf("Opcode function is: %s with %04x\n",__func__,op);}

void (*cmd_pointer[0x10000])(uint16_t opcode);
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

    typedef struct {
        void (*op_fn)(void);
        uint16_t op_name;
    } cmd_pr;

    //cmd_pr *cmd_pointer[0x10000];


    //int i;
    for (int i=0x0000 ; i< 0x1000; i++) cmd_pointer[i] = op0NNN;
    for (int i=0x1000 ; i< 0x2000; i++) cmd_pointer[i] = op1NNN;
    for (int i=0x2000 ; i< 0x3000; i++) cmd_pointer[i] = op2NNN;
    for (int i=0x3000 ; i< 0x4000; i++) cmd_pointer[i] = op3NNN;
    for (int i=0x4000 ; i< 0x5000; i++) cmd_pointer[i] = op4NNN;
    for (int i=0x5000 ; i< 0x6000; i++) cmd_pointer[i] = op5NNN;
    for (int i=0x6000 ; i< 0x7000; i++) cmd_pointer[i] = op6NNN;
    for (int i=0x7000 ; i< 0x8000; i++) cmd_pointer[i] = op7NNN;
    for (int i=0x8000 ; i< 0x9000; i++) cmd_pointer[i] = op8NNN;
    for (int i=0x9000 ; i< 0xA000; i++) cmd_pointer[i] = op9NNN;
    for (int i=0xA000 ; i< 0xB000; i++) cmd_pointer[i] = opANNN;
    for (int i=0xB000 ; i< 0xC000; i++) cmd_pointer[i] = opBNNN;
    for (int i=0xC000 ; i< 0xD000; i++) cmd_pointer[i] = opCNNN;
    for (int i=0xD000 ; i< 0xE000; i++) cmd_pointer[i] = opDNNN;
    for (int i=0xE000 ; i< 0xF000; i++) cmd_pointer[i] = opENNN;
    for (int i=0xF000 ; i< 0xFF66; i++) cmd_pointer[i] = opFNNN;
    cmd_pointer[0x00E0] = op00E0;
    cmd_pointer[0x00EE] = op00EE;

    uint16_t opcode=0xc000;
    cmd_pointer[opcode](opcode);
    printf("chip initialized\n");
    return c8;
}
void execute(uint16_t op) {
    if (cmd_pointer[op])
       cmd_pointer[op](op);
    else
        printf("%04x not implmenent/undefined\n", op);
}
