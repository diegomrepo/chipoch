#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip-8.h"
#include "video.h"

void init_rand() {
    /* Intializes random number generator */
    time_t t;
    srand((unsigned)time(&t));
}

void load_rom(chp8_t *c8, const char *rom) {
    printf("Enter %s\n", __func__);
    FILE *pFile;
    // unsigned char buffer[4096] = {0};
    uint8_t *buffer = &(c8->memory[START_ADDR]);
    long lSize, result;

    pFile = fopen(rom, "rb");
    if (pFile == NULL) {
        fputs("File error\n", stderr);
        exit(EXIT_FAILURE);
    } else {
        fputs("File OK\n", stderr);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    if (START_ADDR + lSize >= RAM_LIMIT)
        exit(EXIT_FAILURE);

    // copy the file into the buffer:
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

    // terminate
    fclose(pFile);
    printf("Close %s\n", __func__);
}

void dump_memory(chp8_t *c8, bool dump_all) {
    unsigned int start, limit;
    if (dump_all) {
        start = 0;
        limit = RAM_LIMIT;
    } else {
        start = START_ADDR;
        limit = 0x0000;
    }
    printf("Enter %s\n", __func__);
    for (int16_t i = start; i < RAM_LIMIT;
         i = i + 2) { /*TODO: Clean variables */
        if (c8->memory[i] == limit)
            continue;
        printf("%04x: %02x%02x  \n", i, c8->memory[i], c8->memory[i + 1]);
    }
    // putchar('\n');
    printf("Leave %s\n", __func__);
}
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"

void dump_video(chp8_t *c8) {
    printf("Enter %s\n", __func__);
    char *color[2] = {KNRM, KRED};
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 64; ++j) {
            printf("%s%02d%02d%s ", color[c8->video[i * 64 + j]], i, j,
                   color[0]);
        }
        putchar('\n');
    }
    printf("Leave %s\n", __func__);
}
void decrement_timer(chp8_t *c8, uint32_t *last_tick) {
#define DELAY 1
    uint32_t now = SDL_GetTicks();
    if (DELAY >= 17) {
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
        uint32_t ticks_since = diff / 16;
        c8->dly_timer = (uint8_t)fmax(0, c8->dly_timer - ticks_since);
        c8->snd_timer = (uint8_t)fmax(0, c8->snd_timer - ticks_since);
    }
    *last_tick = now;
}

int main(int argc, char **argv) {
    uint32_t last_tick = SDL_GetTicks();
    printf("Enter %s\n", __func__);
    const char *rom;
    if (argv[1]) {
        rom = argv[1];
    } else {
        // rom = "/home/dmarfil/projects/chip-8/roms/PONG";
        rom = "/home/dmarfil/projects/chip-8/roms/PONG2";
        // rom =
        // "/home/dmarfil/projects/chip-8/3rd-party/chip-8/roms/Fishie.ch8"; rom
        // = "/home/dmarfil/projects/chip-8/roms4/test_opcode.ch8"; rom =
        // "/home/dmarfil/projects/chip-8/roms/PONG"; rom =
        // "/home/dmarfil/projects/chip-8/roms2/IBMLogo.ch8"; rom =
        // "/home/dmarfil/projects/chip-8/roms2/jason.ch8"; rom =
        // "/home/dmarfil/Downloads/BC_test.ch8"; rom =
        // "/home/dmarfil/projects/chip-8/roms2/RandomNumberTestMatthewMikolay2010.ch8";
    }

    init_rand();
    chp8_t *c8 = init_chip(); /*TODO: Load rom on init */
    init_video(&win, &renderer);
    load_rom(c8, rom);
    dump_memory(c8, true);
    bool exit_event = false;
    while (!exit_event) {
        // c8->video[yy*63+xx] = bb;
        // dump_video(c8);
        cycle(c8, &exit_event);
        poll_events(&exit_event, c8, false);
        draw_fb(c8, renderer);
        decrement_timer(c8, &last_tick);
        SDL_Delay(DELAY);
    }

    free_video(renderer, win);
    printf("Press ENTER key to Continue\n");char ch; scanf("%c",&ch);
}
