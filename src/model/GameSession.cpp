#include "model/GameSession.h"

#include <algorithm>

namespace isaac::model {

void GameSession::update(float seconds, const GameplayInput& input) {
  snapshot_.elapsedSeconds += std::max(0.F, seconds);
  const auto direction = input.movement.normalized();
  snapshot_.playerPosition += direction * (180.F * seconds);
  snapshot_.playerPosition.x = std::clamp(snapshot_.playerPosition.x, 52.F, 908.F);
  snapshot_.playerPosition.y = std::clamp(snapshot_.playerPosition.y, 92.F, 488.F);
}

}  // namespace isaac::model

