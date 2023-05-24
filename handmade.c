#include<stdio.h>
#include<SDL.h>

#include "./handmade.h"

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
    SDL_Surface* sf_hello_world = check_ptr(SDL_LoadBMP("./hello-world.bmp"));
    SDL_Rect viewport = {0, 0, 25, 25};
    SDL_Rect sqr01 = {0, 0, 50, 50};
    SDL_Rect sqr02 = {50, 50, 50, 50};
    SDL_Event evt;
    char redraw = 1;
    for (;;) {
        SDL_PollEvent(&evt);
        if (evt.type == SDL_QUIT) {
            SDL_Log("Program quit after %i ticks", evt.quit.timestamp);
            break;
        }
        if (evt.type == SDL_WINDOWEVENT
                && evt.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            // viewport.w += 1;
            // viewport.h += 1;
            // surface = check_ptr(SDL_GetWindowSurface(window));
            redraw = 1;
        }
        // SDL_BlitSurface(landscape, &viewport, surface, NULL);
        // SDL_UpdateWindowSurface(window);
        if (redraw) {
            check_code(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
            check_code(SDL_RenderClear(renderer));
            check_code(SDL_SetRenderDrawColor(renderer, 255, 10, 5, 255));
            check_code(SDL_RenderFillRect(renderer, &sqr01));
            check_code(SDL_SetRenderDrawColor(renderer, 10, 255, 5, 255));
            check_code(SDL_RenderFillRect(renderer, &sqr02));
            redraw = 0;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }

	SDL_DestroyWindow(window);
	SDL_Quit();
    return(0);
}
