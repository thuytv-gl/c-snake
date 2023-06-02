#include "handmade.h"

global_variable char running = 1;
global_variable SDL_Texture *arts_texture = NULL;
global_variable Snake snake = {DirectionEast};
const SDL_Rect wall_tile = {
    ICON_SIZE*2,
    ICON_SIZE*2,
    ICON_SIZE,
    ICON_SIZE
};
const SDL_Rect snake_tile = {ICON_SIZE, ICON_SIZE*2, ICON_SIZE, ICON_SIZE};
const SDL_Rect head_tile = {ICON_SIZE, ICON_SIZE*3, ICON_SIZE, ICON_SIZE};
const int snake_len = BOARD_SIZE * BOARD_SIZE;

void scc(char code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

template <typename T> T* scp(T *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

void prepare_wall(SDL_Renderer *renderer) {
    SDL_Rect target_tile = {0, 0, 32, 32};
    for (int i = 0; i < BOARD_SIZE; ++i) {
        target_tile.x = i * 32;
        for (int j = 0; j < BOARD_SIZE; ++j) {
            target_tile.y = j * 32;
            if (i == 0 || j == 0 || i == 19 || j == 19) {
                SDL_RenderCopy(renderer, arts_texture, &wall_tile, &target_tile);
            }
        }
    }
}

void prepare_snake(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.w = ICON_SIZE;
    rect.h = ICON_SIZE;
    for (int i = 0; i < snake_len; ++i) {
        if (snake.body[i].x == 0)
            break;

        rect.x = snake.body[i].x * ICON_SIZE;
        rect.y = snake.body[i].y * ICON_SIZE;
        if (i == 0) 
            SDL_RenderCopy(renderer, arts_texture, &head_tile, &rect);
        else
            SDL_RenderCopy(renderer, arts_texture, &snake_tile, &rect);
    }
}

void prepare_scene(SDL_Renderer *renderer) {
    scc(SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255));
    scc(SDL_RenderFillRect(renderer, NULL));
    prepare_wall(renderer);
    prepare_snake(renderer);
}

void do_render(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

void snake_forward() {
    int tail = -1;
    for (int i = 0; i < snake_len; i++) {
        if (snake.body[i].x == 0) {
            tail = i-1;
            break;
        }
    }

    SDL_Point head = snake.body[0];
    for (;tail > 0; --tail) {
        snake.body[tail].x = snake.body[tail-1].x;
        snake.body[tail].y = snake.body[tail-1].y;
    }
    switch(snake.d)
    {
        case DirectionEast:
        {
            snake.body[0].x = head.x + 1;
        } break;
        case DirectionWest:
        {
            snake.body[0].x = head.x - 1;
        } break;
        case DirectionNorth:
        {
            snake.body[0].y = head.y - 1;
        } break;
        case DirectionSouth:
        {
            snake.body[0].y = head.y + 1;
        } break;
        default: break;
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
                    snake.d = DirectionWest;
                } break;
                case SDLK_d:
                {
                    snake.d = DirectionEast;
                } break;
                case SDLK_w:
                {
                    snake.d = DirectionNorth;
                } break;
                case SDLK_s:
                {
                    snake.d = DirectionSouth;
                } break;
                // case SDLK_SPACE:
                // {
                //     snake_forward();
                // } break;
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
    for (int i = 0; i < 5; ++i) {
        snake.body[i].x = 5 - i;
        snake.body[i].y = 2;
    }

    SDL_Window* window = scp(SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_RESIZABLE));
    SDL_Renderer* renderer = scp(SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED));
    arts_texture = scp(SDL_CreateTextureFromSurface(
                renderer,
                IMG_Load("./fantasy-tileset.png"))); 
    SDL_Event event;
    float speed = 1.3;
    Uint32 timeout = SDL_GetTicks() + speed * 100;
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
        do_render(renderer);
        SDL_Delay(FRAME_RATE);

        if (SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
            timeout = SDL_GetTicks() + speed * 100;
            snake_forward();
        }
    }

    SDL_Log("Q U I T");
	SDL_Quit();
    return(0);
}

