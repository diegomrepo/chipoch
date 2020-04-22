#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "chip-8.h"
#include "video.h"
#include "main.h"

// SDL_Window *win;
// SDL_Renderer *renderer;
// SDL_Rect rect;

void draw_fb(chp8_t *c8, SDL_Renderer *renderer) {
    int color;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 64; ++j) {
            if (c8->video[i * 64 + j] != 0)
                color = 255;
            else
                color = 0;
            draw_pixel(renderer, &(SDL_Rect){.x = j, .y = i}, color);
        }
    SDL_RenderPresent(renderer);
}
void showbits( unsigned int x )
{
    for (int i = (sizeof(uint8_t) * 8) - 1; i >= 0; i--)
       putchar(x & (1u << i) ? '1' : '0');
    printf("\n");
}

void draw8bits_fb(chp8_t *c8, uint8_t sprite, uint8_t y, uint8_t x) {
    //printf("Enter %s\n", __func__);
    uint8_t bit = 0;
    printf("\nSprite: %02x | x: %d | y: %d", sprite, x, y);
    puts("");
    //SDL_Delay(500);

    //c8->video[0] = 1; c8->video[1] = 1; c8->video[2] = 1;
    puts("sprite: ");showbits(sprite);
    //uint8_t sprite_t = 0xaa;
    //printf("SPRITE (I_reg) %04x\n", sprite);
    for (int i = 0; i < 8; i++, bit = 0) {
        //printf("CICLE: %d\n", i);
        //printf("i:%d sprite: %04x\n", i, sprite);
        bit = (sprite >> (7 - i));
        //puts("bit: ");showbits(bit);
        //printf("bit, medium: %04x - ", bit);
        bit = bit & 0xB001;
        //printf("bit, final: %04x - ", bit);
        int tmp = 64 * y + x + i;
        printf("video arr:%d | ",tmp);
        if (c8->video[tmp] && bit)
            c8->V[0xf] = 1;
        int v_tmp = c8->video[tmp] ^ bit;
        printf("video[%d]=%02x - bit=%02x xor v_t=%02x",tmp,c8->video[tmp], bit,v_tmp);
        c8->video[tmp] ^= bit;
        //c8->video[tmp] = 1;
        //sprite_t >>= 1;
        //printf("i:%d sprite: %04x bit:%0x\n", i, sprite, bit);
    }
    dump_video(c8);
    //SDL_Quit();
    //exit(EXIT_SUCCESS);
    //printf("Leave %s\n", __func__);
}
void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect, int color) {
    // printf("Enter %s\n", __func__);
    // printf("x=%d y=%d\n", rect->x, rect->y);
    if (rect->x >= 64 || rect->y >= 32) {
        puts("ERROR: Pixel out of bound");
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderDrawColor(renderer, color, color, color, SDL_ALPHA_OPAQUE);
    // rect->x = ((rand() % S_WIDTH) / 10) * 10;
    rect->x = rect->x * 10;
    // rect->y = ((rand() % S_HEIGHT) / 10) * 10;
    rect->y = rect->y * 10;
    rect->w = 10;
    rect->h = 10;
    SDL_RenderFillRect(renderer, rect);
    // printf("Leave %s\n", __func__);
}

void free_video(SDL_Renderer *renderer, SDL_Window *win) {
    // clr(renderer, BLACK);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

uint8_t poll_events(bool *catched_event, chp8_t *c8, bool wait) {
    SDL_Event event;
    uint8_t k = 255;
    bool inner_wait = wait;
    while (SDL_PollEvent(&event) || inner_wait) {
        if ((SDL_QUIT == (event).type) ||
            (SDL_KEYDOWN == (event).type &&
             SDL_SCANCODE_ESCAPE == (event).key.keysym.scancode)) {
            *catched_event = true;
            break;
        }
            //printf(".");
            if (SDL_KEYDOWN == (event).type) {
                 //printf("/////////////////////////////////////////\n");
                 //printf("CODE: %d\n", (event).key.keysym.scancode);
                 //SDL_Delay(1000);
                /*TODO replace with switch */
                 /*NOTE: Do not use  SDLK shortcut */
                 if ((event).key.keysym.scancode == SDL_SCANCODE_1) k=0x1;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_2) k=0x2;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_3) k=0x3;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_4) k=0xc;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_Q) k=0x4;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_W) k=0x5;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_E) k=0x6;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_R) k=0xd;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_A) k=0x7;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_S) k=0x8;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_D) k=0x9;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_F) k=0xe;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_Z) k=0xa;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_X) k=0x0;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_C) k=0xb;
                 if ((event).key.keysym.scancode == SDL_SCANCODE_V) k=0xf;
                 if (k!=255) inner_wait = false;
            }
    }
    //if (wait) return k;
    //else {c8->key[k & 0xf]=k; return 255;}
    return c8->key[k & 0xf]=k;
}

void clr(SDL_Renderer *renderer, uint8_t t_color, chp8_t *c8) {
    printf("color: %i\n", t_color);
    /* TODO: Use SDL_Color struct? */
    SDL_SetRenderDrawColor(renderer, t_color, t_color, t_color,
                           SDL_ALPHA_OPAQUE);
    // SDL_SetRenderDrawColor(renderer, 100, t_color, t_color, 255);
    // SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    memset(c8->video, 0, sizeof(c8->video));
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
    // clr(*renderer, C_GREY);
}
