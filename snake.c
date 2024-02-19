#include "snake.h"

global int score = 0;
global bit running = 1;
global Snake snake;
global SDL_Point fruit = { 0, 0 };
global TTF_Font *font;
global SDL_Renderer *renderer;
global SDL_Window* window;

void scc(char code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void quit() {
  running = 0;
}

bit is_snake_body(SDL_Point *point) {
    for (int i = 0; i < snake.len; i++) {
        SDL_Point *cell = &(snake.body[i]);
        if (cell->x == point->x && cell->y == point->y) {
          return 1;
        }
    }
    return 0;
}

void spawn_fruit() {
    fruit.x = randnum(1, BOARD_SIZE-1);
    fruit.y = randnum(1, BOARD_SIZE-1);
    while(is_snake_body(&fruit)) {
      fruit.x = randnum(1, BOARD_SIZE-1);
      fruit.y = randnum(1, BOARD_SIZE-1);
    }
}

void snake_eat_fruit() {
    SDL_Point *head = &(snake.body[0]);
    if (head->x == fruit.x && head->y == fruit.y) {
        if (snake.len < MAX_LEN) {
          SDL_Point tail = snake.body[snake.len-1];
          snake.body[snake.len].x = tail.x;
          snake.body[snake.len].y = tail.y;
          snake.len++;
        }

        score++;
        spawn_fruit();
    }
}

/**
 * Function: snake_forward.
 * Description: advance snake, eat fruit and grow, check wall and self collision.
 * returns:
 *  0 - advanced.
 *  1 - self collision.
 *  2 - wall collision.
 */
int snake_forward() {
    int tail = snake.len-1;
    SDL_Point *head = &(snake.body[0]);

    // remove the tail
    for (;tail > 0; --tail) {
        snake.body[tail].x = snake.body[tail-1].x;
        snake.body[tail].y = snake.body[tail-1].y;
    }

    if (snake.direction.x != 0) {
      (*head).x = head->x + snake.direction.x;
    } else if (snake.direction.y != 0) {
      (*head).y = head->y + snake.direction.y;
    }

    // check wall collision
    if (head->x == 0 || head->y == 0 || head->x == BOARD_SIZE || head->y == BOARD_SIZE) {
      return SNAKE_WALL_COLLISION;
    }

    // check self collision
    for (int i = 1; i < snake.len; i++) {
        SDL_Point *cell = &(snake.body[i]);
        if (cell->x == head->x && cell->y == head->y) {
          return SNAKE_SELF_COLLISION;
        }
    }

    snake_eat_fruit();
    return 0;
}

void render_wall() {
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

void render_fruit() {
    SDL_Rect rect_fruit = {
        fruit.x*TILE_SIZE,
        fruit.y*TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect_fruit);
}

void render_snake() {
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

const char score_format[] = "Point: %d";
char score_buffer[sizeof(score) + sizeof(score_format)];
void render_score() {
  SDL_Surface* text;
  // Set color to black
  SDL_Color color = { 0, 255, 0 };
  sprintf(score_buffer, score_format, score);

  text = TTF_RenderText_Solid(font, score_buffer, color );
  if (!text) {
    exit(1);
  }
  SDL_Texture* text_texture;

  text_texture = SDL_CreateTextureFromSurface(renderer, text );

  SDL_Rect dest = { 0, BOARD_SIZE * TILE_SIZE + TILE_SIZE * 2, text->w, text->h };

  SDL_RenderCopy( renderer, text_texture, NULL, &dest);
}

void draw_scene() {
    SDL_RenderPresent(renderer);
}

void render_frame() {
    scc(SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255));
    scc(SDL_RenderFillRect(renderer, NULL));
    render_wall();
    render_fruit();
    render_snake();
    render_score();

    draw_scene();
}

void window_event_handler(SDL_Event *event) {
    switch(event->type)
    {
        case SDL_QUIT:
        {
            // TODO(thuytv): should we X out? or do something else?
            (void)quit();
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
            SDL_Point *direction = &snake.direction;
            switch(event->key.keysym.sym) {
                case SDLK_a:
                {
                  if (direction->y != 0) {
                    direction->x = -1;
                    direction->y = 0;
                  }
                } break;
                case SDLK_d:
                {
                  if (direction->y != 0) {
                    direction->x = 1;
                    direction->y = 0;
                  }
                } break;
                case SDLK_w:
                {
                  if (direction->x != 0) {
                    direction->y = -1;
                    direction->x = 0;
                  }
                } break;
                case SDLK_s:
                {
                  if (direction->x != 0) {
                    direction->y = 1;
                    direction->x = 0;
                  }
                } break;
                case SDLK_q:
                {
                  (void)quit();
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
    scc(TTF_Init());
    font = TTF_OpenFont("font.ttf", 24);
    window = SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1,
                SDL_RENDERER_ACCELERATED);

    if (!renderer || !font || !window) {
      exit(1);
    }
}

void init_snake() {
    snake.direction.x = 1;
    snake.direction.y = 0;
    for (int i = 0; i < INITIAL_LEN; ++i) {
        snake.body[i].x = INITIAL_LEN - i;
        snake.body[i].y = 2;
        snake.len++;
    }
}

int main(int argc, char** argv) {
    init_sdl();
    init_snake();
    srand(time(0));
    spawn_fruit();

    SDL_Event event;
    float speed = 0.5;
    Uint32 timeout = SDL_GetTicks() + speed * 100;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_USEREVENT) {
                if (event.user.code == HMD_EVENT_QUIT) {
                    break;
                }
            }
            (void)window_event_handler(&event);
        }

        render_frame();
        SDL_Delay(FRAME_RATE);

        if (SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
            timeout = SDL_GetTicks() + speed * 100;
            if (snake_forward() != 0) {
              quit();
            }
        }
    }

    SDL_Log("Q U I T");
	  SDL_Quit();
    return(0);
}

