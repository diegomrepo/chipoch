#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip-8.h"

void clr(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void init_video(SDL_Window **win, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }
    if (SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_RESIZABLE, win, renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        exit(3);
    }
    atexit(SDL_Quit);
    clr(*renderer);
}

void poll_events(bool *catched_event) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if ((SDL_QUIT == (event).type) ||
            (SDL_KEYDOWN == (event).type &&
             SDL_SCANCODE_ESCAPE == (event).key.keysym.scancode)) {
            *catched_event = true;
            break;
        }
    }
}

void free_video(SDL_Renderer *renderer, SDL_Window *win) {
    clr(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    (*rect).x = (rand() % S_WIDTH / 10) * 10;
    (*rect).y = (rand() % S_HEIGHT / 10) * 10;
    (*rect).w = 10;
    (*rect).h = 10;
    SDL_RenderFillRect(renderer, rect);
    SDL_RenderPresent(renderer);
}

 void init_rand() {
    /* Intializes random number generator */
    time_t t;
    srand((unsigned) time(&t));
 }
 void load_rom(chp8_t * c8, const char *rom) {
    FILE *pFile;
    unsigned char buffer[4096] = {0};
    long lSize, result;

    //const char *rom_path = "roms3/fishie.ch8";

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
        printf("%02x%02x  ", buffer[i], buffer[i+1]);
    putchar('\n');
    // terminate
    fclose(pFile);
 }

int main() {
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Rect rect;

    chp8_t * c8;
    //const char *rom = "roms3/fishie.ch8";

    init_rand();
    c8 = init_chip(); /*TODO: Load rom on init */
    load_rom(c8, "../roms3/fishie.ch8");
    init_video(&win, &renderer);

    bool catched_event = false;
    while (!catched_event) {
        draw_pixel(renderer, &rect); //SDL_Delay(1);
        poll_events(&catched_event);
        SDL_Delay(100);
    }
    free_video(renderer, win);
}
