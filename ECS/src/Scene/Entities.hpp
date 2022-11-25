#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include "../entt.hpp"
#include "Scene.h"

class Entity
{
  public:
    Entity(entt::entity e, Scene* s) {
      std::cout << "Entity Created" << std::endl;

      handle = e;
      scene = s;
    }

    ~Entity() {
      std::cout << "Entity Destroyed" << std::endl;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
      return scene->r.emplace<T>(handle, std::forward<Args>(args)...);
    }

    template<typename T>
    void removeComponent() {
      scene->r.remove<T>(handle);
    }

  private:
    entt::entity handle;
    Scene* scene;
};

#endif