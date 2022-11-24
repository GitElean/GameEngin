#include <SDL2/SDL.h>

class Game {
  public:
    Game();
    ~Game();

    void init(const char* title, int width, int height);
    void setup();
    void frameStart();
    void frameEnd();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();
    bool loadMedia();

  private:
    int counter;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    // for frame management
    int FPS;
    Uint32 frameStartTimestamp;
    Uint32 frameEndTimestamp;
    float frameDuration;
    float dT; // time in miliseconds since last frame
};