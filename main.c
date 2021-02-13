
/*
 * chipoch is a CHIP-8 emulator done in C
 * Copyright (C) 2020 Diego Marfil <diegomrepo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip-8.h"
#include "video.h"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define DEFAULT_ROM "../roms/PONG2"

extern uint32_t delay;

/**
 * Intializes random number generator
 * @note Needed for 'hardware' simulated RAND
 *
 */
void init_rand()
{
	time_t t;
	srand((unsigned)time(&t));
}

/**
 * Loads ROM File
 *
 * @param *c8 Pointer to main chip structure
 * @param *rom Static relative or full path to ROM
 * @note Exit code, constant that is platform independent 
 */
void load_rom(chp8_t *c8, const char *rom)
{
	printf("Enter %s\n", __func__);
	FILE *pFile;

	uint8_t *buffer = &(c8->memory[START_ADDR]);
	long lSize, result;

	pFile = fopen(rom, "rb");
	if (pFile == NULL) {
		fputs("File error\n", stderr);
		exit(EXIT_FAILURE);
	} else {
		fputs("File OK\n", stderr);
	}

	/* obtain file size: */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);
	if (START_ADDR + lSize >= RAM_LIMIT)
		exit(EXIT_FAILURE);

	/* copy the file into the buffer: */
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) {
		fputs("Reading error\n", stderr);
		exit(EXIT_FAILURE);
	} else {
		fputs("Reading OK\n", stderr);
	}

	/* the whole file is now loaded in the memory buffer. */
	printf("Size of buffer: %ld bytes \n", sizeof(buffer));
	for (int i = 0; i < lSize; i = i + 2)
		printf("%02x%02x  ", buffer[i], buffer[i + 1]);
	putchar('\n');

	/* terminate */
	fclose(pFile);
	printf("Close %s\n", __func__);
}

/**
 * Dumps RAM. Optionally dumps interpreter ROM,
 * and font data.
 *
 * @param *c8 Pointer to main chip structure
 * @param dump_all flag to dump entire memory or just RAM
 */
void dump_memory(chp8_t *c8, bool dump_all)
{
	unsigned int start, limit;
	if (dump_all) {
		start = 0;
		limit = RAM_LIMIT;
	} else {
		start = START_ADDR;
		limit = 0x0000;
	}
	printf("Enter %s\n", __func__);
	for (int16_t i = start; i < RAM_LIMIT; i = i + 2) {
		if (c8->memory[i] == limit)
			continue;
		printf("%04x: %02x%02x  \n", i, c8->memory[i],
		       c8->memory[i + 1]);
	}
	printf("Leave %s\n", __func__);
}

/**
 * Dumps display memory (framebuffer)
 *
 * @param *c8 Pointer to main chip structure
 */
void dump_video(chp8_t *c8)
{
	printf("Enter %s\n", __func__);
	char *color[2] = { KNRM, KRED };
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 64; ++j) {
			printf("%s%02d%02d%s ", color[c8->video[i * 64 + j]], i,
			       j, color[0]);
		}
		putchar('\n');
	}
	printf("Leave %s\n", __func__);
}

/**
 * Chip-8 timer decrement function.
 * Uses SDL tick feature.
 *
 * @param *c8 Pointer to main chip structure
 * @return last_tick self explanatory
 */
void decrement_timer(chp8_t *c8, uint32_t *last_tick)
{
	uint32_t now = SDL_GetTicks();
	if (delay >= 17) {
		if (c8->dly_timer)
			c8->dly_timer -= 1;
		else
			c8->dly_timer -= 0;
		if (c8->snd_timer)
			c8->snd_timer -= 1;
		else
			c8->snd_timer -= 0;
	} else {
		uint32_t diff = *last_tick - now;
		uint32_t ticks_since = (uint32_t)((float)diff / 5);
		c8->dly_timer = (uint8_t)fmax(0, c8->dly_timer - ticks_since);
		c8->snd_timer = (uint8_t)fmax(0, c8->snd_timer - ticks_since);
	}
	*last_tick = now;
}

/**
 * Application's main entry point.
 *
 * @param argc Argument counter
 * @param argv Argument vector
 * @return EXIT_SUCCESS if final point is reached
 *         without errors
 * @note ROM filename and tick delay can be given
 *       as parameter
 */
int main(int argc, char **argv)
{
	uint32_t last_tick = SDL_GetTicks();
	printf("Enter %s\n", __func__);
	delay = 1;
	const char *rom;

	if (argv[1]) {
		puts("Setting ROM from args");
		rom = argv[1];
	} else {
		rom = DEFAULT_ROM;
	}
	if (argc >= 2 && argv[2]) {
		puts("Setting delay from args");
		delay = atoi(argv[2]);
	}

	init_rand();
	chp8_t *c8 = init_chip();
	init_video(&win, &renderer);
	load_rom(c8, rom);
	dump_memory(c8, true);
	bool exit_event = false;

	while (!exit_event) {
		cycle(c8, &exit_event);
		poll_events(&exit_event, c8, false);
		draw_fb(c8, renderer);
		decrement_timer(c8, &last_tick);
		SDL_Delay(delay);
	}

	free_video(renderer, win);
	exit(EXIT_SUCCESS);
}
