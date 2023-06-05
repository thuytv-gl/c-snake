#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

// CONFIGS
#define WINDOW_TITLE     "Handmade"
#define WINDOW_WIDTH     1080
#define WINDOW_HEIGHT    720
#define FRAME_RATE       1000/60
#define ICON_SIZE        32
#define BOARD_SIZE       20

// EVENTS
#define HMD_EVENT_QUIT 0

#define local_persist static
#define global_variable static
#define randnum(min, max) \
    ((rand() % (int)(((max) + 1) - (min))) + (min))

enum Direction {
    DirectionEast,
    DirectionNorth,
    DirectionSouth,
    DirectionWest
};

typedef struct {
    Direction d;
    SDL_Point body[BOARD_SIZE * BOARD_SIZE];
    int len;
} Snake;
