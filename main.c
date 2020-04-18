#define DELAY 1000
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
    FILE *pFile;
    // unsigned char buffer[4096] = {0};
    uint8_t *buffer = &(c8->memory[START_ADDR]);
    long lSize, result;

    // const char *rom_path = "roms3/fishie.ch8";

    pFile = fopen(rom, "rb");
    if (pFile == NULL) {
        fputs("File error\n", stderr);
        exit(1);
    } else {
        fputs("File OK\n", stderr);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    if (START_ADDR + lSize >= RAM_LIMIT)
        exit(4);

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        fputs("Reading error\n", stderr);
        exit(3);
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
}

void dump_memory(chp8_t *c8) {
    for (int i = 0; i < RAM_LIMIT; i = i + 2)
        printf("%02x%02x  ", c8->memory[i], c8->memory[i + 1]);
    putchar('\n');
}

int main() {
    // const char *rom = "../roms/PONG";
    const char *rom = "../roms2/RandomNumberTestMatthewMikolay2010.ch8";

    init_rand();
    c8 = init_chip(); /*TODO: Load rom on init */
    // load_rom(c8, "../roms2/RandomNumberTestMatthewMikolay2010.ch8");
    init_video(&win, &renderer);
    SDL_Delay(DELAY);
    load_rom(c8, rom);
    bool catched_event = false;
    while (!catched_event) {
        cycle(c8, &catched_event);
        draw_pixel(renderer, &rect);
        SDL_Delay(DELAY);
        poll_events(&catched_event);
    }
    free_video(renderer, win);
}
