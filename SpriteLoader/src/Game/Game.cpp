#include <iostream>
#include <string>
#include "Game.h"
#include <SDL2/SDL.h>

SDL_Rect rec1;
SDL_Rect rec2;


Game::Game(){
    FPS = 60;
    frameDuration = (1.0f / FPS)* 1000.0f;
    counter = 0;
}

Game::~Game(){

}


void Game ::init(const char* title, int width, int height){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 0, 0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    isRunning = true;
}

void Game::handleEvents()
{
  std::cout << "Game Handling events..." << std::endl;

  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      isRunning = false;
    }
  }
}

void Game::frameStart(){
    std::cout <<"...start"<<std::endl;
    frameStartTimeStamp = SDL_GetTicks();

    deltaTime = frameEndTimeStamp - frameStartTimeStamp;

}

void Game::frameEnd(){
    std::cout <<"...end"<<std::endl;
    frameStartTimeStamp = SDL_GetTicks();
    float actualFrameDuration = frameEndTimeStamp - frameStartTimeStamp;
    if (actualFrameDuration < frameDuration)
    {
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;
    
}

void Game ::update(){
    std::cout <<"Game updating"<<std::endl;

}
void Game ::render(){
    std::cout <<"Game rendering"<<std::endl;

    SDL_SetRenderDrawColor(renderer, 55, 100, 100, 1);
    SDL_RenderClear(renderer);

    /*
    SDL_Surface* surface = IMG_Load("./assets/kroniireference1.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr , &rec2);

    SDL_DestroyTexture(texture);
*/
    SDL_RenderPresent(renderer);
    

}
void Game ::setup(){
    rec1.x = 400;
    rec1.y = 300;
    rec1.h = 500;
    rec1.w = 240;

    rec2.x = 200;
    rec2.y = 100;
    rec2.h = 400;
    rec2.w = 200;

    std::cout <<"Game setuping"<<std::endl;

}
void Game ::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout <<"Game cleaning"<<std::endl;
}
bool Game ::running(){
    return isRunning;
}