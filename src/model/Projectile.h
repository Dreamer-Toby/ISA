#pragma once

#include "common/MathTypes.h"

namespace isaac::model {

struct Projectile {
  common::Vec2 position;
  common::Vec2 velocity;
  float damage{};
  float lifetime{1.6F};
  bool friendly{true};
  bool alive{true};

  void update(float seconds) {
    position += velocity * seconds;
    lifetime -= seconds;
    alive = alive && lifetime > 0.F && position.x > 35.F && position.x < 925.F &&
            position.y > 75.F && position.y < 505.F;
  }
};

}  // namespace isaac::model

