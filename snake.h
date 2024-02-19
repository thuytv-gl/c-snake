#ifndef SNAKE_H
#define SNAKE_H 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_ttf.h"

#define private static
#define bit char

// CONFIGS
#define WINDOW_TITLE     "Handmade Snake"
#define WINDOW_WIDTH     1080
#define WINDOW_HEIGHT    720
#define FRAME_RATE       1000/30
#define TILE_SIZE        10
#define BOARD_SIZE       40
#define MAX_LEN          BOARD_SIZE * BOARD_SIZE - BOARD_SIZE
#define INITIAL_LEN      15

// EVENTS
#define HMD_EVENT_QUIT 0

// HELPERS
#define local_persist static
#define global static
#define randnum(min, max) \
    ((rand() % (int)(((max) + 1) - (min))) + (min))

// COLLISIONS
#define SNAKE_SELF_COLLISION 1
#define SNAKE_WALL_COLLISION 2

typedef struct {
    SDL_Point direction;
    SDL_Point body[BOARD_SIZE * BOARD_SIZE];
    int len;
} Snake;

#endif /* SNAKE_H */
