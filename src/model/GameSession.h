#pragma once

#include "common/MathTypes.h"
#include "model/Player.h"
#include "model/Projectile.h"
#include "model/Level.h"
#include "model/EnemySystem.h"
#include "model/ItemSystem.h"
#include "model/BossSystem.h"

#include <cstddef>
#include <string>
#include <vector>

namespace isaac::model {

struct GameplayInput {
  common::Vec2 movement;
  common::Vec2 shooting;
  bool useBomb{};
  bool useActive{};
  bool interact{};
};

struct ProjectileSnapshot {
  common::Vec2 position;
  bool friendly{};
};

struct RoomSnapshot {
  int id{};
  common::RoomType type{};
  int gridX{};
  int gridY{};
  bool visited{};
  bool revealed{};
  bool current{};
  bool cleared{};
};

struct EnemySnapshot {
  common::Vec2 position;
  std::string id;
};

struct PickupSnapshot {
  common::Vec2 position;
  common::PickupType type{};
};

struct BossSnapshot {
  common::Vec2 position;
  std::string id;
  int phase{};
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
  std::vector<RoomSnapshot> rooms;
  std::vector<EnemySnapshot> enemies;
  std::vector<PickupSnapshot> pickups;
  std::vector<BossSnapshot> bosses;
  int floor{1};
  common::RoomType roomType{common::RoomType::Normal};
  bool roomCleared{true};
  std::size_t totalShots{};
  float elapsedSeconds{};
  bool playerDead{};
  bool devilRoomAvailable{};
  bool runCompleted{};
};

class GameSession {
 public:
  explicit GameSession(float devilRoomRoll = 0.2F);
  void selectCharacter(std::size_t index);
  void update(float seconds, const GameplayInput& input);
  [[nodiscard]] const SessionSnapshot& snapshot() const { return snapshot_; }
  [[nodiscard]] Player& player() { return player_; }
  [[nodiscard]] const Player& player() const { return player_; }
  [[nodiscard]] const std::vector<Projectile>& projectiles() const { return projectiles_; }
  [[nodiscard]] Level& level() { return level_; }
  [[nodiscard]] const Level& level() const { return level_; }
  [[nodiscard]] EnemySystem& enemySystem() { return enemies_; }
  [[nodiscard]] const std::vector<Pickup>& pickups() const { return pickups_; }

 private:
  void rebuildSnapshot();
  Player player_;
  Level level_;
  EnemySystem enemies_;
  std::vector<Pickup> pickups_;
  ItemSystem items_;
  BossSystem bosses_;
  float devilRoomRoll_{};
  bool bossEncounterActive_{};
  bool bossRewardResolved_{};
  bool runCompleted_{};
  std::vector<Projectile> projectiles_;
  SessionSnapshot snapshot_{};
};

}  // namespace isaac::model
