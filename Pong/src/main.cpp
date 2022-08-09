#include <SDL2/SDL.h>
#include <Game.h>
#include <stdio.h>

Game *game = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 680;

int main( int argc, char* args[] )
{

    game = new Game();

    game->init("PONG", SCREEN_WIDTH, SCREEN_HEIGHT);

    game->setup();

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