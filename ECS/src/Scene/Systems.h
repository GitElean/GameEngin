#ifndef Systems_H
#define Systems_H
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Systems {
  protected:
    Scene* scene;

  public:
    void setScene(Scene* s) { scene = s; };
};

class SetupSystems : public Systems {
  public:
    virtual void run() = 0;
};

class InputSystems : public Systems {
  public:
    virtual void run(SDL_Event* event) = 0;
};

class UpdateSystems : public Systems {
  public:
    virtual void run(double dT) = 0;
};

class RenderSystems : public Systems {
  public:
    virtual void run(SDL_Renderer* renderer) = 0;
};

class MusicSystems : public Systems {
  public:
    virtual void run(SDL_Renderer* renderer) = 0;
}

#endif