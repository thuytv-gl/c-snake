#include<stdio.h>
#include<SDL.h>

#include "handmade.h"

void check_code(char code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void* check_ptr(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

int main(int argc, char** argv) {
    check_code(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window* window = check_ptr(SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                C_WINDOW_FLAGS));
    SDL_Renderer* renderer = check_ptr(SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED));
    SDL_Surface* surface = check_ptr(SDL_GetWindowSurface(window));
    SDL_Texture* texture = check_ptr(SDL_CreateTextureFromSurface(renderer,
                surface));
    SDL_Event evt;
    short red = 50;
    short blue = 100;
    for (;;) {
        SDL_PollEvent(&evt);
        if (evt.type == SDL_QUIT) {
            SDL_Log("Program quit after %i ticks", evt.quit.timestamp);
            break;
        }
        if (evt.type == SDL_WINDOWEVENT
                && evt.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            red += 10;
            blue += 5;
        }
        check_code(SDL_SetRenderDrawColor(renderer, red, 44, blue, 255));
        check_code(SDL_RenderClear(renderer));
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

	SDL_DestroyWindow(window);
	SDL_Quit();
    return(0);
}
