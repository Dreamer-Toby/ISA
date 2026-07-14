#pragma once

#include "model/GameSessionInterface.h"
#include "model/Player.h"
#include "model/Projectile.h"
#include "model/Level.h"
#include "model/EnemySystem.h"
#include "model/ItemSystem.h"
#include "model/BossSystem.h"

#include <vector>

namespace isaac::model {

class GameSession final : public GameSessionInterface {
 public:
  explicit GameSession(float devilRoomRoll = 0.2F);
  void selectCharacter(std::size_t index) override;
  void update(float seconds, const GameplayInput& input) override;
  [[nodiscard]] const SessionSnapshot& snapshot() const override { return snapshot_; }
  [[nodiscard]] std::vector<ModelEvent> drainEvents() override;
  [[nodiscard]] std::size_t selectableCharacterCount() const override;
  [[nodiscard]] CharacterSummary selectableCharacter(std::size_t index) const override;
  [[nodiscard]] Player& player() { return player_; }
  [[nodiscard]] const Player& player() const { return player_; }
  [[nodiscard]] const std::vector<Projectile>& projectiles() const { return projectiles_; }
  [[nodiscard]] Level& level() { return level_; }
  [[nodiscard]] const Level& level() const { return level_; }
  [[nodiscard]] EnemySystem& enemySystem() { return enemies_; }

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
  std::vector<ModelEvent> pendingEvents_;
};

}  // namespace isaac::model
