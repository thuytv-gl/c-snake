#define WINDOW_WIDTH    1080
#define WINDOW_HEIGHT   720
#define WINDOW_TITLE   "Handmade"

const int C_RENDER_FLAGS = SDL_RENDERER_ACCELERATED;
const int C_WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;

typedef struct {
    SDL_Window*     window;
    SDL_Surface*    surface;
} App;

