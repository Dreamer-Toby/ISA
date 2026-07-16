#pragma once

#include "common/MathTypes.h"

#include <cmath>

namespace isaac::model {

struct Projectile {
  common::Vec2 position;
  common::Vec2 velocity;
  float damage{};
  float lifetime{1.6F};
  bool friendly{true};
  bool alive{true};
  bool sineWave{};
  float age{};

  void update(float seconds) {
    common::Vec2 waveOffset;
    if (sineWave) {
      constexpr float WaveAmplitude = 28.F;
      constexpr float WaveFrequency = 10.F;
      const auto direction = velocity.normalized();
      const common::Vec2 perpendicular{-direction.y, direction.x};
      const float previous = std::sin(age * WaveFrequency) * WaveAmplitude;
      age += seconds;
      const float next = std::sin(age * WaveFrequency) * WaveAmplitude;
      waveOffset = perpendicular * (next - previous);
    } else {
      age += seconds;
    }
    position += velocity * seconds + waveOffset;
    lifetime -= seconds;
    alive = alive && lifetime > 0.F && position.x > 35.F && position.x < 925.F &&
            position.y > 75.F && position.y < 505.F;
  }
};

}  // namespace isaac::model
