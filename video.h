#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#define WHITE 255
#define BLACK 0
#define C_GREY 100

SDL_Window *win;
SDL_Renderer *renderer;
SDL_Rect rect;

void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect);
void free_video(SDL_Renderer *renderer, SDL_Window *win);
void poll_events(bool *catched_event);
void clr(SDL_Renderer *renderer, uint8_t color);
void init_video(SDL_Window **win, SDL_Renderer **renderer);
