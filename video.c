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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "chip-8.h"
#include "main.h"
#include "video.h"

/**
 * Walks the framebuffer and renders
 * one pixel at the time.
 *
 * @param *c8 Pointer to main chip structure
 * @param *renderer SDL's rendering context
 */
void draw_fb(chp8_t *c8, SDL_Renderer *renderer)
{
	int color;
	for (int i = 0; i < 32; ++i)
		for (int j = 0; j < 64; ++j) {
			if (c8->video[i * 64 + j] != 0)
				color = 255;
			else
				color = 0;
			draw_pixel(renderer, &(SDL_Rect){ .x = j, .y = i },
				   color);
		}
	SDL_RenderPresent(renderer);
}

/**
 * Prints to screen the binary representation of
 * a non-negative integer
 *
 * @param decimal self explanatory
 */
void showbits(unsigned int decimal)
{
	for (int i = (sizeof(uint8_t) * 8) - 1; i >= 0; i--)
		putchar(decimal & (1u << i) ? '1' : '0');
	printf("\n");
}

/**
 * Draw a pixel inside display memory (framebuffer)
 *
 * @param *c8 Pointer to main chip structure
 * @param sprite type of supported sprite as defined
 *               by chip-8 architecture
 * @param x horizontal position
 * @param y vertical position
 * @param erased mark pixel as erased
 *
 * @note chip8's pixels are not the same as fb_pixels
 */
void draw8bits_fb(chp8_t *c8, uint8_t sprite, uint8_t x, uint8_t y,
		  bool *erased)
{
	uint8_t bit = 0;
	for (int i = 0; i < 8; i++, bit = 0) {
		bit = (sprite >> (7 - i));
		bit = bit & 0xB001;
		int tmp = 64 * y + x + i;
		if (c8->video[tmp] && bit)
			*erased = 1;
		c8->video[tmp] ^= bit;
	}
}

/**
 * Draw a pixel inside the SDL context
 *
 * @param *renderer SDL's rendering context
 * @param *rect rectangle SDL struct
 * @param color color in RGB format
 */
void draw_pixel(SDL_Renderer *renderer, SDL_Rect *rect, int color)
{
	if (rect->x >= 64 || rect->y >= 32) {
		puts("ERROR: Pixel out of bound");
		exit(EXIT_FAILURE);
	}
	SDL_SetRenderDrawColor(renderer, color, color, color, SDL_ALPHA_OPAQUE);
	rect->x = rect->x * 10;
	rect->y = rect->y * 10;
	rect->w = 10;
	rect->h = 10;
	SDL_RenderFillRect(renderer, rect);
}

/**
 * Frees SDL's video resources.
 *
 * @param *renderer SDL's rendering context
 * @param *win SDL's window
 */
void free_video(SDL_Renderer *renderer, SDL_Window *win)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

/**
 * Polls the SLD event queue 
 *
 * @param *catched_event is there an event?
 * @param *c8 Pointer to main chip structure
 * @param wait Custom delay
 *
 * @return Key pressed
 */
uint8_t poll_events(bool *catched_event, chp8_t *c8, bool wait)
{
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
		if (SDL_KEYDOWN == event.type) {
			printf("pressed %c", event.key.keysym.scancode);
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_1:
				k = 0x1;
				break;
			case SDL_SCANCODE_MINUS:
				delay -= 1, printf("delay -1");
				break;
			case SDL_SCANCODE_PERIOD:
				delay += 1, printf("delay +1");
				break;
			case SDL_SCANCODE_2:
				k = 0x2;
				break;
			case SDL_SCANCODE_3:
				k = 0x3;
				break;
			case SDL_SCANCODE_4:
				k = 0xc;
				break;
			case SDL_SCANCODE_Q:
				k = 0x4;
				break;
			case SDL_SCANCODE_W:
				k = 0x5;
				break;
			case SDL_SCANCODE_E:
				k = 0x6;
				break;
			case SDL_SCANCODE_R:
				k = 0xd;
				break;
			case SDL_SCANCODE_A:
				k = 0x7;
				break;
			case SDL_SCANCODE_S:
				k = 0x8;
				break;
			case SDL_SCANCODE_D:
				k = 0x9;
				break;
			case SDL_SCANCODE_F:
				k = 0xe;
				break;
			case SDL_SCANCODE_Z:
				k = 0xa;
				break;
			case SDL_SCANCODE_X:
				k = 0x0;
				break;
			case SDL_SCANCODE_C:
				k = 0xb;
				break;
			case SDL_SCANCODE_V:
				k = 0xf;
				break;
			default:
				printf("Key not asigned");
			}
			if (k != 255)
				inner_wait = false;
		} else if (SDL_KEYUP == event.type) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_1:
				c8->key[0x1] = 255;
				break;
			case SDL_SCANCODE_2:
				c8->key[0x2] = 255;
				break;
			case SDL_SCANCODE_3:
				c8->key[0x3] = 255;
				break;
			case SDL_SCANCODE_4:
				c8->key[0xc] = 255;
				break;
			case SDL_SCANCODE_Q:
				c8->key[0x4] = 255;
				break;
			case SDL_SCANCODE_W:
				c8->key[0x5] = 255;
				break;
			case SDL_SCANCODE_E:
				c8->key[0x6] = 255;
				break;
			case SDL_SCANCODE_R:
				c8->key[0xd] = 255;
				break;
			case SDL_SCANCODE_A:
				c8->key[0x7] = 255;
				break;
			case SDL_SCANCODE_S:
				c8->key[0x8] = 255;
				break;
			case SDL_SCANCODE_D:
				c8->key[0x9] = 255;
				break;
			case SDL_SCANCODE_F:
				c8->key[0xe] = 255;
				break;
			case SDL_SCANCODE_Z:
				c8->key[0xa] = 255;
				break;
			case SDL_SCANCODE_X:
				c8->key[0x0] = 255;
				break;
			case SDL_SCANCODE_C:
				c8->key[0xb] = 255;
				break;
			case SDL_SCANCODE_V:
				c8->key[0xf] = 255;
				break;
			default:
				printf("Key not asigned");
			}
		}
	}
	return c8->key[k & 0xf] = k;
}

/**
 * Clears the screen
 *
 * @param *c8 Pointer to main chip structure
 * @param t_color custom clear color
 * @param *renderer SDL's rendering context
 *
 * @note the SDL's screen not the fb
 */
void clr(SDL_Renderer *renderer, uint8_t t_color, chp8_t *c8)
{
	printf("color: %i\n", t_color);
	/* TODO: Use SDL_Color struct? */
	SDL_SetRenderDrawColor(renderer, t_color, t_color, t_color,
			       SDL_ALPHA_OPAQUE);
	memset(c8->video, 0, sizeof(c8->video));
	SDL_RenderClear(renderer);
}

/**
 * Initalizes SDL video
 *
 * @param *win SDL's window
 * @param *renderer SDL's rendering context
 *
 * @note this is SDL boilerplate code
 */
void init_video(SDL_Window **win, SDL_Renderer **renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(1);
	}
	if (SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_RESIZABLE, win,
					renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			     "Couldn't create window and renderer: %s",
			     SDL_GetError());
		exit(3);
	}
	atexit(SDL_Quit);
}
