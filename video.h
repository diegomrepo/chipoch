#pragma once
#include "chip-8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#define WHITE 255
#define BLACK 0
#define C_GREY 100

SDL_Window *win;
SDL_Renderer *renderer;
SDL_Rect rect;

void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect, int color);
void free_video(SDL_Renderer *renderer, SDL_Window *win);
uint8_t poll_events(bool *catched_event, chp8_t *c8, bool wait);
void clr(SDL_Renderer *renderer, uint8_t color, chp8_t *c8);
void init_video(SDL_Window **win, SDL_Renderer **renderer);
void draw_fb(chp8_t *c8, SDL_Renderer *renderer);
void draw8bits_fb(chp8_t *c8, uint8_t sprite, uint8_t x, uint8_t y);
