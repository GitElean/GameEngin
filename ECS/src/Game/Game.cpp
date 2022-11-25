#include <iostream>
#include "Game.h"
#include <SDL2/SDL.h>
#include "../entt.hpp"


#include "Scene/Scene.h"

#include "Scene/Entities.hpp"
#include "Scene/Components.hpp"
#include "Scene/Systemss.hpp"

entt::registry reg;
entt::enttity pad1, pad2;


struct PlayerComponent{
  int playerNum;
};

struct PositionComponent {
  int x, y;
};

struct VelocityComponent {
  int x, y;
};

struct CubeComponent {
  int w, h;
};

PositionComponent playerSpawnPosition = {20, 20};
VelocityComponent playerSpawnVelocity = {200, 200};
CubeComponent playerRect = {200, 20};

int screen_width;
int screen_height;

SDL_Rect sandbag;
SDL_Rect player1;
SDL_Rect player2;

int speed = 300;
int dx = 1;
int dy = 1;
int player_Speed=20;

void createPlayerEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect, PlayerComponent player)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<PlayerComponent>(e, player);  

    if (player.playerID == 1){
        pad1 = e;
    }
    else{
        pad2 = e;
    }

}




Game::Game(){
    FPS = 60;
    frameDuration = (1.0f / FPS)* 1000.0f;
    counter = 0;
    deltaTime = 0;
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
      case SDLK_UP:
        if (player2.y - player_Speed >= 0)
        {
        player2.y -= player_Speed;
        }
        
        break;
      case SDLK_DOWN:
        if (player2.y + player2.h + player_Speed <= screen_height)
        {
        player2.y += player_Speed;
        }
        break;
      case SDLK_w:
        if (player1.y - player_Speed >= 0)
        {
        player1.y -= player_Speed;
        }
        break;
      case SDLK_s:
        if (player1.y + player1.h + player_Speed <= screen_height)
        {
        player1.y += player_Speed;
        }
        break;
      
      }
    }


    
  }
}

void Game::frameStart(){
    std::cout <<"...start"<<std::endl;
    frameStartTimeStamp = SDL_GetTicks();

    if (frameEndTimeStamp!=0)
    {
            deltaTime =  (frameStartTimeStamp - frameEndTimeStamp)/1000.0f ;
    }
    
}

void Game::frameEnd(){
    std::cout <<"...end"<<std::endl;
    frameEndTimeStamp = SDL_GetTicks();
    float actualFrameDuration = frameStartTimeStamp - frameEndTimeStamp;
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

    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 1);
    SDL_RenderFillRect(renderer, &sandbag);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);


    SDL_RenderPresent(renderer);

}


void Game ::setup(){
    std::cout <<"Game setuping"<<std::endl;
    createPlayerEntity();
    sandbag.x = screen_width/3;
    sandbag.y = screen_height/4;
    sandbag.h = 100;
    sandbag.w = 50;

    player1.x = 10;
    player1.y = 300;
    player1.h = 100;
    player1.w = 30;

    player2.x = 800;
    player2.y = 300;
    player2.h = 100;
    player2.w = 30;

}
void Game ::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout <<"Game Over"<<std::endl;
}
bool Game ::running(){
    return isRunning;
}