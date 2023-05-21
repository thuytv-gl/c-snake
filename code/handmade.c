#include<stdio.h>
#include<SDL.h>

#include "handmade.h"

char setup(App* app) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[Error]: Failed to initialize SDL, %s", SDL_GetError());
        return 0;
    }
    app->window = SDL_CreateWindow(WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            C_WINDOW_FLAGS);
    if (app->window == NULL) {
        printf("[Error]: Failed to create window, %s", SDL_GetError());
        return 0;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    return 1;
}

void cleanup(App* app) {
    SDL_FreeSurface(app->surface);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
}

int main(int argc, char* args[]) {
    App app;
    short red = 50;
    short blue = 100;
    if (setup(&app)) {
        SDL_Event evt;

        for (;;) {
            app.surface = SDL_GetWindowSurface(app.window);
            SDL_FillRect(app.surface, NULL, SDL_MapRGB(app.surface->format, red, 128, blue));
            SDL_UpdateWindowSurface(app.window);
            SDL_PollEvent(&evt);
            if (evt.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", evt.quit.timestamp);
                break;
            }
            if (evt.type == SDL_WINDOWEVENT) {
                switch(evt.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        red += 10;
                        blue += 5;
                    } break;
                    default: break;
                }
            }
            SDL_Delay(16);
        }
    }
    cleanup(&app);
    return(0);
}
