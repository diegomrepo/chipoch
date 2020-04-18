#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "chip-8.h"
#include "video.h"

SDL_Window *win;
SDL_Renderer *renderer;
SDL_Rect rect;

void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    (*rect).x = (rand() % S_WIDTH / 10) * 10;
    (*rect).y = (rand() % S_HEIGHT / 10) * 10;
    (*rect).w = 10;
    (*rect).h = 10;
    SDL_RenderFillRect(renderer, rect);
    SDL_RenderPresent(renderer);
}

void free_video(SDL_Renderer *renderer, SDL_Window *win) {
    // clr(renderer, BLACK);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
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

void clr(SDL_Renderer *renderer, uint8_t t_color) {
    printf("color: %i\n", t_color);
    /* TODO: Use SDL_Color struct? */
    SDL_SetRenderDrawColor(renderer, t_color, t_color, t_color,
                           SDL_ALPHA_OPAQUE);
    // SDL_SetRenderDrawColor(renderer, 100, t_color, t_color, 255);
    // SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderClear(renderer);
}

void init_video(SDL_Window **win, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }
    if (SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_RESIZABLE, win,
                                    renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create window and renderer: %s", SDL_GetError());
        exit(3);
    }
    atexit(SDL_Quit);
    clr(*renderer, C_GREY);
}
