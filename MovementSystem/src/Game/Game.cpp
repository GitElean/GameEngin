#include <iostream>
#include <string>
#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  

int screen_width;
int screen_height;


SDL_Rect sandbag;
SDL_Rect player;
SDL_Rect background;

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
    screen_height = height;
    screen_width = width;
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


    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_RIGHT:
        player.x += 30;
        
        
        break;
      case SDLK_LEFT:
        player.x -= 30;
        break;
      
      case SDLK_UP:
        player.y -= 160;
        break;
    
      /*case SDLK_z:
        Uint32 color = SDL_SetRenderDrawColor(renderer, 255, 10, 10, 1);
        int SDL_FillRect(renderer, &sandbag, color);
        break;*/
      }
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

    
    SDL_Surface* surBackground = IMG_Load("./assets/background.png");
    SDL_Texture* texBackground = SDL_CreateTextureFromSurface(renderer, surBackground);
    SDL_FreeSurface(surBackground);

    SDL_RenderCopy(renderer, texBackground, nullptr , &background);
    SDL_DestroyTexture(texBackground);


    SDL_Surface* surface = IMG_Load("./assets/kronii.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr , &player);

    SDL_DestroyTexture(texture);

    SDL_Surface* surface2 = IMG_Load("./assets/sandbag.png");
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

    SDL_FreeSurface(surface2);

    SDL_RenderCopy(renderer, texture2, nullptr , &sandbag);

    SDL_DestroyTexture(texture2);



    SDL_RenderPresent(renderer);
    

}
void Game ::setup(){
    sandbag.x = 1100;
    sandbag.y = 250;
    sandbag.h = 390;
    sandbag.w = 220;

    player.x = 100;
    player.y = 250;
    player.h = 396;
    player.w = 275;

    background.x = -screen_width/2.5;
    background.y = 0;
    background.h = screen_height*1.1;
    background.w = screen_width*1.8;

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