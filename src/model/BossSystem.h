#pragma once

#include "common/MathTypes.h"
#include "model/Player.h"
#include "model/Projectile.h"

#include <array>
#include <string_view>
#include <vector>

namespace isaac::model {

enum class BossPattern { LeapVolley, OrbitBurst, AxisDash };

struct BossDefinition {
  std::string_view id;
  std::string_view displayName;
  float health;
  BossPattern pattern;
};

struct Boss {
  std::size_t definitionIndex{};
  common::Vec2 position;
  float health{};
  int phase{1};
  float phaseTimer{};
  float attackTimer{};
};

class BossCatalog {
 public:
  static const std::array<BossDefinition, 3>& all();
};

class BossSystem {
 public:
  void spawnForFloor(int floor);
  bool update(float seconds, Player& player, std::vector<Projectile>& projectiles);
  [[nodiscard]] bool empty() const { return bosses_.empty(); }
  [[nodiscard]] const std::vector<Boss>& bosses() const { return bosses_; }
  [[nodiscard]] std::vector<Boss>& bosses() { return bosses_; }

 private:
  std::vector<Boss> bosses_;
};

class DevilRoomPolicy {
 public:
  static constexpr float Chance = 0.35F;
  static bool opens(float unitRoll) { return unitRoll >= 0.F && unitRoll < Chance; }
};

}  // namespace isaac::model
