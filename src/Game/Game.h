#include <SDL2/SDL.h>

class Game
{
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

    int FPS;
    Uint32 frameStartTimeStamp;
    Uint32 frameEndTimeStamp;
    float frameDuration;
    int counter;

    float deltaTime;

public:
    Game(/* args */);
    ~Game();

    void init(const char* title, int width, int height);
    void handleEvents();
    void frameStart();
    void frameEnd();

    void update();
    void render();
    void setup();
    void clean();
    bool running();

};


