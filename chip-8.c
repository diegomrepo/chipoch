#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip-8.h"
#include "video.h"
#define NUM_KEYS 16

void cycle(chp8_t *c8, bool *event)
{
	// dump_memory(c8);
	uint16_t opcode = 0;

	printf("\n\nPC = 0x%04x", c8->pc);
	printf(" | DT = %d", c8->dly_timer);
	printf(" | ST = %d", c8->dly_timer);
	printf(" | I = %04x\n", c8->i_register);

	for (int i = 0; i < NUM_KEYS; i++) {
		printf(" k%d=%02x", i, c8->key[i]);
	}
	puts("");
	for (int i = 0; i < 16; i++) {
		printf(" v%d = %02x", i, c8->V[i]);
	}
	puts("");

	opcode =
		c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1]; // get instrc.
	printf("opcode = 0x%04x\n", opcode);
	if (opcode == 0)
		*event = true;

	c8->pc = c8->pc + 2;
	if (c8->pc > RAM_LIMIT)
		*event = true;
	//    int num = chip_rand();
	execute(c8, opcode);
}

uint8_t chip_rand()
{
	return rand() % 256;
}
void op0NNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	puts("IGNORED: Not in current specification");
	puts("** 0nnn - SYS addr **");
}

void op00E0(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** CLS **\n");
	clr(renderer, BLACK, c8);
}

void op00EE(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 00EE - RET **\n");
	if (c8->sp > STACK_SIZE) { /* 2^N − 1 underflow */
		puts("ERROR: Invalid Operation: Stack empty");
		exit(EXIT_FAILURE);
	}
	c8->pc = c8->stack[c8->sp];
	c8->sp--;
}

void op1NNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** JP addr **\n");
	c8->pc = op & 0x0fff;
	// printf("PC is now %04x\n", op);
}

void op2XNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	/* Change opcode name */
	uint16_t nnn = op & 0x0fff;
	c8->sp++;
	if (c8->sp > STACK_SIZE) {
		printf("ERROR: Stack overflow.");
		exit(EXIT_FAILURE);
	}
	c8->stack[c8->sp] = c8->pc;
	c8->pc = nnn;
	printf("** 2nnn - CALL addr **\n");
}

void op3XNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 3xkk - SE Vx, byte **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t kk = op & 0x00ff;
	if (c8->V[x] == kk)
		c8->pc += 2;
}

void op4XNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 4xkk - SE Vx, byte **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t kk = op & 0xff;
	if (c8->V[x] != kk)
		c8->pc += 2;
}

void op5XY0(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 5xy0 - SE Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	if (c8->V[x] == c8->V[y])
		c8->pc += 2;
}

void op6XNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 6xkk - LD Vx, byte **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t kk = op & 0xff;
	c8->V[x] = kk;
}

void op7XNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 7xkk - ADD Vx, byte **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t kk = op & 0x00ff;
	c8->V[x] = (uint8_t)(c8->V[x] + kk);
}

/* multi 8 */
void op8XN0(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy0 - LD Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	c8->V[x] = c8->V[y];
}

void op8XN1(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy1 - OR Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	c8->V[x] |= c8->V[y];
}

void op8XN2(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy2 - AND Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	c8->V[x] &= c8->V[y];
}

void op8XN3(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy3 - XOR Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	c8->V[x] ^= c8->V[y];
}

void op8XN4(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy4 - ADD Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	uint16_t tmp = c8->V[x] + c8->V[y];
	c8->V[0xf] = (tmp >= 0x100);
	c8->V[x] = (uint8_t)tmp;
}

void op8XN5(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	/*rename*/
	printf("** 8xy5 - SUB Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	int16_t tmp = c8->V[x] - c8->V[y];
	c8->V[0xf] = (tmp >= 0);
	c8->V[x] = (uint8_t)tmp;
}

void op8XN6(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy6 - SHR Vx {, Vy} **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	/* y not used? */
	c8->V[0xf] = c8->V[x] & 1;
	c8->V[x] = c8->V[x] >> 1;
}

void op8XN7(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xy7 - SUBN Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	int16_t tmp = c8->V[y] - c8->V[x];
	c8->V[0xf] = (tmp >= 0);
	c8->V[x] = (uint8_t)tmp;
}

void op8XNE(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 8xyE - SHL Vx {, Vy} **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	/* y not used? */
	// uint8_t msb; = c8->V[x] & 0xb1000;
	// msb; = c8->V[x] & 0xb1000;
	c8->V[0xf] = c8->V[x] >> 7;
	c8->V[x] = c8->V[x] << 1;
}

void op8XYN(chp8_t *c8, uint16_t op)
{
	uint8_t nibble = op & 0xf;
	void (*op8YN_fn[0x10])(chp8_t * c8, uint16_t op) = {
		op8XN0, op8XN1, op8XN2, op8XN3, op8XN4, op8XN5, op8XN6, op8XN7,
		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	op8XNE, NULL
	};
	printf("MULTI-OPCODE: %s, %04x\n", __func__, op);
	op8YN_fn[nibble](c8, op);
}

void op9XY0(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** 9xy0 - SNE Vx, Vy **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t y = (op & 0x00f0) >> 4;
	if (c8->V[x] != c8->V[y])
		c8->pc += 2;
}

void opANNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	c8->i_register = op & 0x0fff;
	puts("** Annn - LD I, addr **");
}

void opBNNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	puts("** Bnnn - JP V0, addr **");
	uint16_t nnn = op & 0x0fff;
	c8->pc =
		(nnn + c8->V[0]) % 0x1000; /* WARNING: Modded to not overflow */
}

void opCXNN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t kk = op & 0xff;
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t rand = chip_rand();
	uint8_t operation = rand & kk;

	// c8->V[x] = chip_rand() & kk;
	// printf("kk: %02x x: %02x, rand(): %02x, operation: %02x", kk, x, rand,
	// operation); puts("");
	c8->V[x] = operation;
	puts("** Cxkk - RND Vx, byte **");
}

void opDXYN(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t n = op & 0x000f;
	uint8_t y = (op & 0x00f0) >> 4;
	uint8_t x = (op & 0x0f00) >> 8;
	uint16_t tmp = c8->i_register;
	bool erased = false;
	if (tmp > MEM_SIZE) {
		printf("ERROR: I_register bigger than MEM_SIZE");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < n; i++, tmp++) {
		draw8bits_fb(c8, c8->memory[tmp], c8->V[x], c8->V[y] + i,
			     &erased);
	}
	c8->V[0xf] = erased;
	puts("** Dxyn - DRW Vx, Vy, nibble **");
}

void opEX9E(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t x = (op & 0x0f00) >> 8;
	puts("** Ex9E - SKP Vx**");
	if (c8->key[c8->V[x]] != 255) {
		c8->pc += 2;
	}
}

void opEXA1(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	puts("** ExA1 - SKNP Vx **");
	uint8_t x = (op & 0x0f00) >> 8;
	if (c8->key[c8->V[x]] == 255) {
		c8->pc += 2;
	}
}

void opFX07(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** Fx07 - LD Vx, DT **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	c8->V[x] = c8->dly_timer;
}

void opFX0A(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** Fx0A - LD Vx, K **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	printf("Waiting for input: ");
	c8->V[x] = poll_events(NULL, c8, true);
}

void opFX15(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** Fx15 - LD DT, Vx **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	// timer
	c8->dly_timer = c8->V[x];
}

void opFX18(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	printf("** Fx18 - LD ST, Vx  **\n");
	uint8_t x = (op & 0x0f00) >> 8;
	c8->snd_timer = c8->V[x];
}

void opFX1E(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	puts("** Fx1E - ADD I, Vx **");
	uint8_t x = (op & 0x0f00) >> 8;
	c8->i_register += c8->V[x];
}

void opFX29(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t x = (op & 0x0f00) >> 8;
	c8->i_register = (c8->V[x] & 0xf) * 5;
	puts("** Fx29 - LD F, Vx**");
}

void opFX33(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	puts("** Fx33 - LD B, Vx **");
	uint8_t x = (op & 0x0f00) >> 8;
	uint8_t vx = c8->V[x];
	c8->memory[c8->i_register] = (vx / 100);
	c8->memory[c8->i_register + 1] = (vx % 100) / 10;
	c8->memory[c8->i_register + 2] = vx % 10;
}

void opFX55(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t x = (op & 0x0f00) >> 8;
	printf("Storing in memory[%04x] all registers until v[%1x]",
	       c8->i_register, x);
	for (int i = 0; i <= x; ++i)
		c8->memory[c8->i_register + i] = c8->V[i];
	/* If I too big, wrap around/modded when overflow ? */
	puts("Fx55 - LD [I], Vx");
}

void opFX65(chp8_t *c8, uint16_t op)
{
	printf("Opcode function is: %s with %04x\n", __func__, op);
	uint8_t x = (op & 0x0f00) >> 8;
	for (int i = 0; i <= x; ++i) {
		printf("Storing in V[%1x] <- %02x | ", i,
		       c8->memory[c8->i_register + i]);
		puts("");
		c8->V[i] = c8->memory[c8->i_register + i];
	}
	/* If I too big, wrap around/modded when overflow ? */
	/* Are the increments alright? */
	puts("** Fx65 - LD Vx, [I] **");
}

void opFXNN(chp8_t *c8, uint16_t op)
{
	uint8_t byte = op & 0x00ff;
	printf("MULTI-OPCODE: %s, %04x\n", __func__, op);
	void (*opFX_fn[0x100])(chp8_t * c8, uint16_t op) = { NULL };
	opFX_fn[0x07] = opFX07;
	opFX_fn[0x0A] = opFX0A;
	opFX_fn[0x15] = opFX15;
	opFX_fn[0x18] = opFX18;
	opFX_fn[0x1E] = opFX1E;
	opFX_fn[0x29] = opFX29;
	opFX_fn[0x33] = opFX33;
	opFX_fn[0x55] = opFX55;
	opFX_fn[0x65] = opFX65;
	opFX_fn[byte](c8, op);
}

void (*cmd_pointer[OPCODE_MAX])(chp8_t *c8, uint16_t opcode) = { NULL };
chp8_t *init_chip()
{
	printf("rand initialized: %d\n", chip_rand());
	chp8_t *c8 = malloc(sizeof(chp8_t));
	memset(c8, 0, sizeof(chp8_t));
	memset(c8->memory, 0, MEM_SIZE); /* best way? */
	memset(c8->video, 0, sizeof(c8->video)); /* best way? */
	memset(c8->V, 0,
	       sizeof(c8->V)); /* TODO: Find out if this is best way */
	memset(c8->key, 255,
	       sizeof(c8->key)); /* TODO: Find out if this is best way */
	memcpy(c8->memory, fontset, sizeof fontset);
	c8->i_register = 0;
	c8->pc = START_ADDR;
	c8->sp = 65535; /*-1*/
	c8->dly_timer = 0;
	c8->snd_timer = 0;

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

void execute(chp8_t *c8, uint16_t op)
{
	if (cmd_pointer[op])
		cmd_pointer[op](c8, op);
	else
		printf("%04x not implmenent/undefined\n", op);
}
