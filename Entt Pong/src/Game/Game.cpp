#include <iostream>
#include "Game.h"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

entt::registry reg;
entt::enttity pad1, pad2;

struct sandbagComponent{
  
};

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

void createsandbagEntity(PositionComponent pos, VelocityComponent vel, CubeComponent rect)
{
    const entt::entity e = mRegistry.create();
    mRegistry.emplace<PositionComponent>(e, pos);
    mRegistry.emplace<VelocityComponent>(e, vel);
    mRegistry.emplace<CubeComponent>(e, rect);  

    mRegistry.emplace<sandbagComponent>(e); 

}


void cubeRenderSystems(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255 ,255, 1);

  const auto view = reg.view<PositionComponent, CubeComponent>();
  for (const entt::entity e : view) {
    const PositionComponent position = view.get<PositionComponent>(e);
    const CubeComponent cube = view.get<CubeComponent>(e);

    SDL_Rect rect = { position.x, position.y, cube.w, cube.h };    
    SDL_RenderFillRect(renderer, &rect);
  }
}

void movementSystems(double dT) {
  auto view = reg.view<PositionComponent, VelocityComponent>();
  for (const entt::entity e : view) {
    PositionComponent& pos = view.get<PositionComponent>(e);
    VelocityComponent& vel = view.get<VelocityComponent>(e);

        
        pos.x += vel.x * dT;
        pos.y += vel.y * dT;
  }
}

void bounceSystems(double dT){
  auto view = reg.view<PositionComponent, VelocityComponent, sandbagComponent, CubeComponent>();
  for (const entt::entity e: view){
     PositionComponent& pos = view.get<PositionComponent>(e);
     VelocityComponent& vel = view.get<VelocityComponent>(e);
     CubeComponent& cub = view.get<CubeComponent>(e);
    if (pos.x <= 0)
    {
      vel.x *= -1;
    }

    if (pos.x + cub.w >= SCREEN_WIDTH)
    {
      vel.x *= -1;
    }

    if (pos.y <= 0)
    {
      vel.y *= -1;
    }

    if (pos.y + cub.h >= SCREEN_HEIGHT)
    {
      vel.y *= -1;
    }
  }
}

bool mouseBounceInputSystems(int x, int y)
{
  auto view = reg.view<PositionComponent, CubeComponent, VelocityComponent>();
  for (const entt::entity e : view) {
    const PositionComponent& pos = view.get<PositionComponent>(e);
    VelocityComponent& vel = view.get<VelocityComponent>(e);
    const CubeComponent& cub = view.get<CubeComponent>(e);
    
    if (
      y >= pos.y &&
      x >= pos.x &&
      y <= pos.y + cub.h &&
      x <= pos.x + cub.w    
    ) {
      vel.x *= -1.2;
      vel.y *= -1.2;
    }
  }
  return true;
}


bool paddleEventSystems(SDL_Event event) {
  auto view = reg.view< VelocityComponent, PlayerComponent, PositionComponent, CubeComponent>();
  for (const entt::entity e : view){
    VelocityComponent& vel = view.get<VelocityComponent>(e);
    const PlayerComponent player = view.get<PlayerComponent>(e);
    const PositionComponent pos = view.get<PositionComponent>(e);
    const CubeComponent cube = view.get<CubeComponent>(e);
    int id = player.playerNum;
    switch( event.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                      if (id == 1){
                        if(pos.x >= 0){
                          vel.x = -200;
                        } else {
                          vel.x = 0;
                        }
                      }
                      break;
                    case SDLK_RIGHT:
                      if (id == 1){
                        if(pos.x + 100  <= SCREEN_WIDTH){
                          vel.x = 200;  
                        } else{
                          vel.x = 0;
                        }
                      }
                      break;
                    case SDLK_a:
                      if (id == 2){
                        if(pos.x >= 0){
                          vel.x = -200;
                        } else {
                          vel.x = 0;
                        }
                      }
                      break;
                    case SDLK_d:
                      if(id == 2){
                        if(pos.x + 100  <= SCREEN_WIDTH){
                          vel.x = 200;  
                        } else{
                          vel.x = 0;
                        }
                      }
                      break;
                }
                break;
            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        if( vel.x < 0 )
                            vel.x = 0;
                        break;
                    case SDLK_RIGHT:
                        if( vel.x > 0 )
                            vel.x = 0;
                        break;
                    case SDLK_a:
                      if (vel.x < 0){
                        vel.x = 0;
                      }
                      break;
                    case SDLK_d:
                      if(vel.x > 0){
                        vel.x = 0;
                      }
                      break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
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
    
    if (sandbag.y + sandbag.h >= screen_height)
    {
        dy *= -1; 
    }
    
    if (sandbag.y + sandbag.h <= 0)
    {
        dy *= -1; 
    }

    if (
      sandbag.x + sandbag.w >= player2.x &&
      sandbag.y + sandbag.h >= player2.y &&
      sandbag.y <= player2.y + player2.h
    )
    {
      dx*=-1.5;
      dy*=-1.5;
    }

    if (
      sandbag.y + sandbag.h >= player1.y &&
      sandbag.x + sandbag.w >= player1.x &&
      sandbag.x <= player1.x + player1.w
      )
    {
      dx*=-1.5;
      dy*=-1.5;

    }
    
    

    if (sandbag.x + sandbag.w >= screen_width)
    {
        isRunning = false;
    }

    if (sandbag.x + sandbag.w <= 0)
    {
        isRunning = false;
    }
    
    sandbag.x +=speed * dx * deltaTime;
    sandbag.y +=speed * dy * deltaTime;
    std::cout<<sandbag.x<<std::endl;
    std::cout<<sandbag.y<<std::endl;
    std::cout<<deltaTime<<std::endl;

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
    sandbag.x = screen_width/2;
    sandbag.y = screen_height/2;
    sandbag.h = 20;
    sandbag.w = 20;

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