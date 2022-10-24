#include <SDL2/SDL.h>
#include <Game.h>
#include <stdio.h>


Game *game = nullptr;

//Screen dimension constantsa
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 680;

int main( int argc, char* args[] )
{

    game = new Game();

    game->init("Sprite Renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

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