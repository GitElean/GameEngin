#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "../entt.hpp"


class Entity;
class SetupSystems;
class InputSystems;
class UpdateSystems;
class RenderSystems;

class Scene {
  private:
    std::vector<SetupSystems*> setupSystemss;
    std::vector<InputSystems*> inputSystemss;
    std::vector<UpdateSystems*> updateSystemss;
    std::vector<RenderSystems*> renderSystemss;

  public:
    Scene(const std::string&);
    ~Scene();

    entt::registry r;

    Entity createEntity(
      const std::string& name = "NO NAME",
      int x = 0,
      int y = 0
    );
    void addSetupSystems(SetupSystems* s);
    void addInputSystems(InputSystems* s);
    void addUpdateSystems(UpdateSystems* s);
    void addRenderSystems(RenderSystems* s);

    void setup();
    void update(double dT);
    void render(SDL_Renderer* renderer);
};

#endif