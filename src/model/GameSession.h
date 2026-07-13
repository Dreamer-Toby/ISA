#pragma once

#include "common/MathTypes.h"

namespace isaac::model {

struct GameplayInput {
  common::Vec2 movement;
  common::Vec2 shooting;
};

struct SessionSnapshot {
  common::Vec2 playerPosition{480.F, 300.F};
  float elapsedSeconds{};
};

class GameSession {
 public:
  void update(float seconds, const GameplayInput& input);
  [[nodiscard]] const SessionSnapshot& snapshot() const { return snapshot_; }

 private:
  SessionSnapshot snapshot_{};
};

}  // namespace isaac::model

