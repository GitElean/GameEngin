#include <iostream>
#include <string>
#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  
#include <SDL2/SDL_mixer.h>
#include <unistd.h>      
#include <thread>
#include <chrono>

int screen_width;
int screen_height;


SDL_Rect sandbag;
SDL_Rect player;
SDL_Rect background;
SDL_Rect hitbox;

Mix_Chunk * backgroundMusic;

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


void Game::initAudio(){
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
  backgroundMusic = Mix_LoadWAV("./assets/CalliopeSong.mp3");
  Mix_AllocateChannels(3);
  Mix_PlayChannel(1,backgroundMusic,-1); 
}


void Game::handleEvents()
{

  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      isRunning = false;
    }

// < y > 
    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_RIGHT:
        if (player.x <= 1366 && player.x != sandbag.x+100 && player.x != sandbag.x-100)
        {
        player.x += 30;
        
        }
          
        
        
        
        break;
      case SDLK_LEFT:
      if (player.x>0 && player.x != sandbag.x)
      {
        player.x -= 30;
      }
      
        
        break;
      
      case SDLK_UP:
        if (player.y >= 250)
        {
            player.y -= screen_height/3.5;
        }
        
        break;
    
      case SDLK_z:
        SDL_SetRenderDrawColor(renderer, 255, 25, 25, 1);
        SDL_RenderDrawRect(renderer, &hitbox);
        SDL_SetRenderDrawColor(renderer, 255, 25, 25, 1);
        SDL_RenderFillRect(renderer, &hitbox);
        //int SDL_FillRect(renderer, &sandbag);
        break;
        case SDLK_DOWN:
          
          if (player.h == 396)
          {
            player.h = player.h/2;
            player.y += player.h;
          
          }else{
            player.h = player.h*2;
            player.y -= player.h/2;
          }
          
        break;
        case SDLK_LSHIFT:
        player.x += 200;
        
        
        break;

        
      }
    }
    
  }
}

void Game::frameStart(){
    frameStartTimeStamp = SDL_GetTicks();

    deltaTime = frameEndTimeStamp - frameStartTimeStamp;

}

void Game::frameEnd(){
    frameStartTimeStamp = SDL_GetTicks();
    float actualFrameDuration = frameEndTimeStamp - frameStartTimeStamp;
    if (actualFrameDuration < frameDuration)
    {
        SDL_Delay(frameDuration - actualFrameDuration);
    }
    counter++;
    
}

void Game ::update(){

    if (player.y<250)
    {
      player.y +=12;
    }
    
    
    

}
void Game ::render(){

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
    
    SDL_SetRenderDrawColor(renderer, 255, 10, 10, 1);
    
    SDL_RenderDrawRect(renderer, &hitbox);
    
    SDL_RenderPresent(renderer);
    
        



}
void Game ::setup(){
    sandbag.x = screen_width/1.3;
    sandbag.y = 250;
    sandbag.h = 390;
    sandbag.w = 220;

    hitbox.x = 1120;
    hitbox.y = 250;
    hitbox.h = 390;
    hitbox.w = sandbag.w*0.38;

    player.x = screen_width/8.5;
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