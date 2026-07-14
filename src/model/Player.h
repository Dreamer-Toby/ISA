#pragma once

#include "common/MathTypes.h"
#include "model/CharacterCatalog.h"
#include "model/Components.h"

namespace isaac::model {

class Player {
 public:
  explicit Player(const CharacterDefinition& definition);
  void move(common::Vec2 direction, float seconds);
  void tick(float seconds);
  bool damage(int amount);
  void resetInvulnerability() { invulnerability_ = 0.F; }
  [[nodiscard]] bool canShoot() { return shooting_.consumeShot(); }
  [[nodiscard]] const CharacterDefinition& definition() const { return definition_; }
  [[nodiscard]] const common::Vec2& position() const { return position_; }
  void setPosition(common::Vec2 position) { position_ = position; }
  [[nodiscard]] HealthComponent& health() { return health_; }
  [[nodiscard]] const HealthComponent& health() const { return health_; }
  [[nodiscard]] ShootComponent& shooting() { return shooting_; }
  [[nodiscard]] const ShootComponent& shooting() const { return shooting_; }
  [[nodiscard]] Inventory& inventory() { return inventory_; }
  [[nodiscard]] const Inventory& inventory() const { return inventory_; }
  void addLuck(float amount) { bonusLuck_ += amount; }
  [[nodiscard]] float luck() const { return definition_.luck + bonusLuck_; }

 private:
  CharacterDefinition definition_;
  common::Vec2 position_{480.F, 300.F};
  HealthComponent health_;
  ShootComponent shooting_;
  Inventory inventory_;
  float invulnerability_{};
  float bonusLuck_{};
};

}  // namespace isaac::model
