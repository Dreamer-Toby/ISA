#include "model/Player.h"

#include <algorithm>

namespace isaac::model {

Player::Player(const CharacterDefinition& definition)
    : definition_(definition), health_(definition.heartContainers, definition.redHearts),
      shooting_(definition.damage, definition.shotsPerSecond, 430.F) {
  inventory_.addCoins(definition.startingCoins);
  if (definition.id == "magdalene") inventory_.setActiveItem("Yum Heart");
}

void Player::move(common::Vec2 direction, float seconds) {
  position_ += direction.normalized() * (definition_.moveSpeed * seconds);
  position_.x = std::clamp(position_.x, 60.F, 900.F);
  position_.y = std::clamp(position_.y, 100.F, 480.F);
}

void Player::tick(float seconds) {
  shooting_.tick(seconds);
  invulnerability_ = std::max(0.F, invulnerability_ - seconds);
}

void Player::damage(int amount) {
  if (invulnerability_ > 0.F) return;
  health_.damage(amount);
  invulnerability_ = 0.7F;
}

}  // namespace isaac::model

