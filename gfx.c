#include <stdio.h>
#define WIDTH 4
#define HEIGHT 3
int main(int argc, char *argv[])
{
	int video[HEIGHT][WIDTH] = { { 0, 1, 2, 3 },
				     { 4, 5, 6, 7 },
				     { 8, 9, 10, 11 } };
	int gfx[HEIGHT * WIDTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			printf("[%d, %d]=%d\n", i, j, video[i][j]);
		}
	}
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			printf("[%d, %d]=%d\n", i, j, gfx[WIDTH * i + j]);
		}
	}
	return 0;
}
