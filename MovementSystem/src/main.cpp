#include <SDL2/SDL.h>
#include <Game.h>
#include <stdio.h>


Game *game = nullptr;

//Screen dimension constantsa
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

int main( int argc, char* args[] )
{

    game = new Game();

    game->init("Sprite Renderer", SCREEN_WIDTH, SCREEN_HEIGHT);
    game->setup();
    game->initAudio();

    while (game->running())
    {
      game->frameStart();
      game->handleEvents();
      game->update();
      game->render();
      game->frameEnd();

    }
    

    game->clean();



    return 0;
}