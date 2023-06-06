#include "snake.h"

global_variable char running = 1;
global_variable Snake snake = {DirectionEast};
global_variable SDL_Point fruit = {0, 0};
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

void snake_forward() {
    int tail = snake.len-1;
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

void spawn_fruit() {
    fruit.x = randnum(1, BOARD_SIZE-1);
    fruit.y = randnum(1, BOARD_SIZE-1);
}

void snake_eat_fruit() {
    SDL_Point *head = &(snake.body[0]);
    if (head->x == fruit.x && head->y == fruit.y) {
        SDL_Point tail = snake.body[snake.len-1];
        snake.body[snake.len].x = tail.x;
        snake.body[snake.len].y = tail.y;

        snake.len++;
        spawn_fruit();
    }
}

void prepare_wall(SDL_Renderer *renderer) {
    SDL_Rect rect_wall = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, 150, 10, 0, 255);
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        rect_wall.x = i * TILE_SIZE;
        for (int j = 0; j <= BOARD_SIZE; ++j) {
            rect_wall.y = j * TILE_SIZE;
            if (i == 0 || j == 0 || i == BOARD_SIZE || j == BOARD_SIZE) {
                SDL_RenderFillRect(renderer, &rect_wall);
            }
        }
    }
}

void prepare_fruit(SDL_Renderer *renderer) {
    SDL_Rect rect_fruit = {
        fruit.x*TILE_SIZE,
        fruit.y*TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect_fruit);
}

void prepare_snake(SDL_Renderer *renderer) {
    SDL_Rect rect_body = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, 100, 255, 50, 255);
    for (int i = snake.len-1; i >= 0; i--) {
        if (snake.body[i].x == 0)
            break;

        rect_body.x = snake.body[i].x * TILE_SIZE;
        rect_body.y = snake.body[i].y * TILE_SIZE;
        if (i == 0) {
            SDL_SetRenderDrawColor(renderer, 50, 150, 0, 255);
            SDL_RenderFillRect(renderer, &rect_body);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 147, 200, 0, 255);
            SDL_RenderFillRect(renderer, &rect_body);
        }
    }
}

void prepare_scene(SDL_Renderer *renderer) {
    scc(SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255));
    scc(SDL_RenderFillRect(renderer, NULL));
    prepare_wall(renderer);
    prepare_fruit(renderer);
    prepare_snake(renderer);
}

void do_render(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
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
                /**
                case SDLK_SPACE:
                {
                    snake_forward();
                } break;
                */
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
    srand(time(NULL));
    init_sdl();
    spawn_fruit();
    for (int i = 0; i < 5; ++i) {
        snake.body[i].x = 5 - i;
        snake.body[i].y = 2;
        snake.len++;
    }
    SDL_Window* window = scp(SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_RESIZABLE));
    SDL_Renderer* renderer = scp(SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED));
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
            snake_eat_fruit();
        }
    }

    SDL_Log("Q U I T");
	SDL_Quit();
    return(0);
}

