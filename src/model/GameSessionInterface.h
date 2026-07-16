#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"

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

struct CharacterSummary {
  std::string id;
  std::string displayName;
  int redHearts{};
  float moveSpeed{};
  float damage{};
  float luck{};
};

enum class ModelEvent { Shot, Hurt, Pickup };

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

struct DoorSnapshot {
  common::Direction direction{common::Direction::Up};
  common::RoomType targetType{common::RoomType::Normal};
  bool locked{};
  bool hidden{};
  bool sealed{};
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
  int keys{2};
  std::string activeItem{"None"};
  float moveSpeed{};
  float damage{};
  float shotsPerSecond{};
  float projectileSpeed{};
  std::vector<ProjectileSnapshot> projectiles;
  std::vector<RoomSnapshot> rooms;
  std::vector<DoorSnapshot> doors;
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
  bool roomRewardCollected{};
  bool runCompleted{};
};

class GameSessionInterface {
 public:
  virtual ~GameSessionInterface() = default;

  virtual void selectCharacter(std::size_t index) = 0;
  virtual void update(float seconds, const GameplayInput& input) = 0;
  [[nodiscard]] virtual const SessionSnapshot& snapshot() const = 0;
  [[nodiscard]] virtual std::vector<ModelEvent> drainEvents() = 0;
  [[nodiscard]] virtual std::size_t selectableCharacterCount() const = 0;
  [[nodiscard]] virtual CharacterSummary selectableCharacter(std::size_t index) const = 0;
};

}  // namespace isaac::model
