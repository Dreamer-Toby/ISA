#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"
#include "model/Player.h"
#include "model/Projectile.h"

#include <array>
#include <string_view>
#include <vector>

namespace isaac::model {

enum class MoveStrategy { Chase, Wander, KeepDistance, Dash, Stationary };
enum class AttackStrategy { Contact, AimedShot, RadialShot };
enum class DropStrategy { None, Breakfast };

struct EnemyDefinition {
  std::string_view id;
  std::string_view displayName;
  float health;
  float speed;
  int contactDamage;
  MoveStrategy movement;
  AttackStrategy attack;
  DropStrategy drop;
};

struct Enemy {
  std::size_t definitionIndex{};
  common::Vec2 position;
  float health{};
  float attackTimer{};
  float moveTimer{};
};

struct Pickup {
  common::PickupType type{};
  common::Vec2 position;
};

class EnemyCatalog {
 public:
  static const std::array<EnemyDefinition, 6>& all();
};

class EnemySystem {
 public:
  void spawnForNormalRoom(int roomId, int floor);
  bool update(float seconds, Player& player, std::vector<Projectile>& projectiles,
              std::vector<Pickup>& pickups);
  [[nodiscard]] const std::vector<Enemy>& enemies() const { return enemies_; }
  [[nodiscard]] std::vector<Enemy>& enemies() { return enemies_; }
  [[nodiscard]] bool empty() const { return enemies_.empty(); }

 private:
  void dropFor(const Enemy& enemy, std::vector<Pickup>& pickups) const;
  std::vector<Enemy> enemies_;
};

}  // namespace isaac::model
