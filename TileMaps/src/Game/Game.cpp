#include <iostream>
#include <cmath>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../entt.hpp"

#include "Game.h"

#include "Scene/Scene.h"

#include "Scene/Entities.hpp"
#include "Scene/Components.hpp"
#include "Scene/Systems.hpp"

int screen_width;
int screen_height;

Game::Game()
{
  FPS = 5;
  frameDuration = (1.0f / FPS) * 1000.0f;  // how many mili seconds in one frame
  std::cout << "Game Object Constructed!" << std::endl;

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

Game::~Game()
{
  std::cout << "Game Object Destroyed!" << std::endl;
}

void Game::init(const char* title, int width, int height)
{
  if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    window = SDL_CreateWindow(title, 0, 0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    std::cout << "REN" << renderer << std::endl;
    IMG_Init(IMG_INIT_PNG);
    std::cout << "Game Start!" << std::endl;

    screen_width = width;
    screen_height = height;

    isRunning = true;
  } else {
    isRunning = false;
  }
  counter = 0;
}

Scene* scene;

void Game::setup()
{
  scene = new Scene("Level1");

  Entity player = scene->createEntity();
  player.addComponent<MovementComponent>(MovementComponent{glm::vec2(50, 50)});
  player.addComponent<ColliderComponent>(ColliderComponent{glm::vec2(50, 50)});

  scene->addSetupSystem(new HelloSystem());
  AutoTileSystem* tilesetSystem = new AutoTileSystem(renderer, window);
  scene->addSetupSystem(tilesetSystem);
  scene->addRenderSystem(tilesetSystem);

  scene->setup();
}

void Game::frameStart()
{
  std::cout << "---- Frame: " << counter << " ----" << std::endl;
  frameStartTimestamp = SDL_GetTicks();
  if (frameEndTimestamp)
  {
    dT = (frameStartTimestamp - frameEndTimestamp) / 1000.0f;
  }
  else
  {
    dT = 0;
  }
}

void Game::frameEnd()
{
  frameEndTimestamp = SDL_GetTicks();

  float actualFrameDuration = frameEndTimestamp - frameStartTimestamp;

  if (actualFrameDuration < frameDuration)
  {
    SDL_Delay(frameDuration - actualFrameDuration);
  }

  counter++;
  std::cout << " " << std::endl;
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
      // bounceInputSystem(event.key.keysym.sym);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      // mouseBounceInputSystem(event.motion.x, event.motion.y);
    }
  }
}

void Game::update()
{
  std::cout << "Game Updating.." << std::endl;

  scene->update(dT);
}

// SDL_Rect s2 = { 250, 0, 240, 240 };

void Game::render()
{
  std::cout << "Game Rendering..." << std::endl;

  SDL_SetRenderDrawColor(renderer, 0, 0 , 0, 1);

  SDL_RenderClear(renderer);

  // SDL_RenderCopy(renderer, newTexture, NULL, &s2);

  scene->render(renderer);

  SDL_RenderPresent(renderer);
}

void Game::clean()
{
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
	IMG_Quit();
  SDL_Quit();
  std::cout << "Game Over." << std::endl;
}

bool Game::running()
{
  return isRunning;
}
