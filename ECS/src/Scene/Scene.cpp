#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "Scene.h"

#include "Entities.hpp"
#include "Components.hpp"
#include "Systems.h"


Scene::Scene(const std::string& name)
{
  std::cout << "Scene " << name << "constructed!" << std::endl;
}

Scene::~Scene()
{
  std::cout << "Scene Destroyed!" << std::endl;
}

Entity Scene::createEntity(const std::string& name, int x, int y)
{
  Entity entity = { r.create(), this };
  entity.addComponent<TagComponent>(name);
  entity.addComponent<TransformComponent>(glm::vec2(x, y),glm::vec2(1, 1),0.0d);

  return entity;
}

void Scene::addSetupSystems(SetupSystems* s)
{
  s->setScene(this);
  setupSystemss.push_back(s);
}

void Scene::addInputSystems(InputSystems* s)
{
  s->setScene(this);
  inputSystemss.push_back(s);
}

void Scene::addUpdateSystems(UpdateSystems* s)
{
  s->setScene(this);
  updateSystemss.push_back(s);
}

void Scene::addRenderSystems(RenderSystems* s)
{
  s->setScene(this);
  renderSystemss.push_back(s);
}

void Scene::setup()
{
  std::cout << "Scene Setup" << std::endl;
  
  for (SetupSystems* sys: setupSystemss)
  {
    sys->run();
  }
}

void Scene::update(double dT)
{
  std::cout << "Scene Update" << std::endl;
  
  for (UpdateSystems* sys: updateSystemss)
  {
    sys->run(dT);
  }
}

void Scene::render(SDL_Renderer* renderer)
{
  std::cout << "Scene Render" << std::endl;
  
  for (RenderSystems* sys: renderSystemss)
  {
    sys->run(renderer);
  }
}