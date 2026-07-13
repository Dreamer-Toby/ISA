#pragma once

#include "common/MathTypes.h"
#include "model/Player.h"
#include "model/Projectile.h"

#include <cstddef>
#include <string>
#include <vector>

namespace isaac::model {

struct GameplayInput {
  common::Vec2 movement;
  common::Vec2 shooting;
  bool useBomb{};
  bool useActive{};
};

struct ProjectileSnapshot {
  common::Vec2 position;
  bool friendly{};
};

struct SessionSnapshot {
  common::Vec2 playerPosition{480.F, 300.F};
  std::string characterId{"isaac"};
  int redHearts{3};
  int shields{};
  int coins{};
  int bombs{1};
  int keys{1};
  std::string activeItem{"None"};
  std::vector<ProjectileSnapshot> projectiles;
  std::size_t totalShots{};
  float elapsedSeconds{};
  bool playerDead{};
};

class GameSession {
 public:
  GameSession();
  void selectCharacter(std::size_t index);
  void update(float seconds, const GameplayInput& input);
  [[nodiscard]] const SessionSnapshot& snapshot() const { return snapshot_; }
  [[nodiscard]] Player& player() { return player_; }
  [[nodiscard]] const Player& player() const { return player_; }
  [[nodiscard]] const std::vector<Projectile>& projectiles() const { return projectiles_; }

 private:
  void rebuildSnapshot();
  Player player_;
  std::vector<Projectile> projectiles_;
  SessionSnapshot snapshot_{};
};

}  // namespace isaac::model

