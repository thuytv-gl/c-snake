#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

// CONFIGS
#define WINDOW_TITLE   "Handmade"
#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   720
#define FRAME_RATE   1000/60
#define ICON_SIZE 32

// EVENTS
#define HMD_EVENT_QUIT 0

#define local_persist static
#define global_variable static


const int C_WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;

global_variable char running = 1;
global_variable SDL_Texture *arts_texture = NULL;
global_variable SDL_Rect snake_body[] = {{0, 0, 32, 32}, {32, 0, 32, 32}, {}};

void scc(char code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

template <typename T>
T* scp(T *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

void prepare_scene(SDL_Renderer *renderer) {
    scc(SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255));
    scc(SDL_RenderFillRect(renderer, NULL));
}

void render_wall(SDL_Renderer *renderer) {
    SDL_Rect src_tile = {ICON_SIZE*2, ICON_SIZE*2, ICON_SIZE, ICON_SIZE};
    SDL_Rect target_tile = {0, 0, 32, 32};
    for (int i = 0; i < 20; ++i) {
        target_tile.x = i * 32;
        for (int j = 0; j < 20; ++j) {
            target_tile.y = j * 32;
            if (i == 0 || j == 0 || i == 19 || j == 19) {
                SDL_RenderCopy(renderer, arts_texture, &src_tile, &target_tile);
            }
        }
    }
}

void render_snake(SDL_Renderer *renderer) {
    SDL_Rect src_tile = {ICON_SIZE, ICON_SIZE*2, ICON_SIZE, ICON_SIZE};
    int snake_len = sizeof(snake_body)/sizeof(SDL_Rect);
    for (int i = 0; i < snake_len; ++i) {
        SDL_RenderCopy(renderer, arts_texture, &src_tile, &snake_body[i]);
    }
}

void do_render(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

void snake_forward() {
    int snake_len = sizeof(snake_body)/sizeof(SDL_Rect);
    for (int i = 0; i < snake_len; ++i) {
        snake_body[i].x += 32;
    }
}

void window_event_handler(SDL_Window *window, SDL_Renderer *renderer,
        SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_QUIT:
        {
            // TODO(thuytv): should we X out? or do something else?
            running = 0;
        } break;
        case SDL_WINDOWEVENT: {
            switch(event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                {
                } break;
                default: break;
            }
        } break;
        case SDL_KEYDOWN:
        {
            switch(event->key.keysym.sym) {
                case SDLK_a:
                {
                    snake_body[0].x -= 32;
                } break;
                case SDLK_d:
                {
                    snake_forward();
                } break;
                case SDLK_w:
                {
                    snake_body[0].y -= 32;
                } break;
                case SDLK_s:
                {
                    snake_body[0].y += 32;
                } break;
                default: break;
            }
        } break;
        default: break;
    }
}

void init_sdl() {
    scc(SDL_Init(SDL_INIT_VIDEO));
}

int main(int argc, char** argv) {
    init_sdl();

    SDL_Window* window = scp(SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                C_WINDOW_FLAGS));
    SDL_Renderer* renderer = scp(SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED));
    arts_texture = scp(SDL_CreateTextureFromSurface(
                renderer,
                IMG_Load("./fantasy-tileset.png"))); 
    SDL_Event event;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_USEREVENT) {
                if (event.user.code == HMD_EVENT_QUIT) {
                    break;
                }
            }
            window_event_handler(window, renderer, &event);
        }

        prepare_scene(renderer);
        render_wall(renderer);
        render_snake(renderer);
        do_render(renderer);
        SDL_Delay(FRAME_RATE);
    }

    SDL_Log("Q U I T");
	SDL_Quit();
    return(0);
}

