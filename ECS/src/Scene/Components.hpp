#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <string>

struct TagComponent {
  std::string tag;
};

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
};

struct MovementComponent {
  glm::vec2 velocity;
};

struct ColliderComponent {
  glm::vec2 size;
};

#endif