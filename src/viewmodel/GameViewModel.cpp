#include "viewmodel/GameViewModel.h"

#include "model/CharacterCatalog.h"

#include <sstream>

namespace isaac::viewmodel {

void GameViewModel::update(float seconds, const InputCommand& command) {
  const bool confirmPressed = command.confirm && !confirmWasDown_;
  const bool pausePressed = command.pause && !pauseWasDown_;
  confirmWasDown_ = command.confirm;
  pauseWasDown_ = command.pause;

  if (screen_ == common::ScreenState::Start && confirmPressed) {
    screen_ = common::ScreenState::CharacterSelect;
  } else if (screen_ == common::ScreenState::CharacterSelect && confirmPressed) {
    session_.selectCharacter(selectedCharacter_);
    screen_ = common::ScreenState::Playing;
  } else if (screen_ == common::ScreenState::Playing && pausePressed) {
    screen_ = common::ScreenState::Paused;
  } else if (screen_ == common::ScreenState::Paused && pausePressed) {
    screen_ = common::ScreenState::Playing;
  }

  if (screen_ == common::ScreenState::Playing) {
    session_.update(seconds, model::GameplayInput{command.movement, command.shooting,
                                                  command.useBomb, command.useActive, command.confirm});
  }

  const auto& snapshot = session_.snapshot();
  if (screen_ == common::ScreenState::Playing && snapshot.playerDead) screen_ = common::ScreenState::Defeat;
  if (screen_ == common::ScreenState::Playing && snapshot.runCompleted) screen_ = common::ScreenState::Victory;

  if (screen_ == common::ScreenState::CharacterSelect && !command.confirm) {
    if (command.movement.x > 0.5F) selectedCharacter_ = (selectedCharacter_ + 1) % 4;
    if (command.movement.x < -0.5F) selectedCharacter_ = (selectedCharacter_ + 3) % 4;
  }
}

DisplayState GameViewModel::displayState() const {
  DisplayState result;
  result.screen = screen_;
  const auto& selected = model::CharacterCatalog::at(selectedCharacter_);
  result.selectionName = std::string(selected.displayName);
  std::ostringstream stats;
  stats << "HP " << selected.redHearts << " SPD " << static_cast<int>(selected.moveSpeed)
        << " DMG " << selected.damage << " LUCK " << selected.luck;
  result.selectionStats = stats.str();
  const auto& state = session_.snapshot();
  result.entities.push_back({common::EntityKind::Player, state.playerPosition, 18.F, state.characterId});
  for (const auto& projectile : state.projectiles) {
    result.entities.push_back({projectile.friendly ? common::EntityKind::PlayerProjectile
                                                   : common::EntityKind::EnemyProjectile,
                               projectile.position, 6.F, "tear"});
  }
  for (const auto& enemy : state.enemies) {
    result.entities.push_back({common::EntityKind::Enemy, enemy.position, 16.F, enemy.id});
  }
  for (const auto& boss : state.bosses) {
    result.entities.push_back({common::EntityKind::Boss, boss.position,
                               boss.id == "moms_leg" ? 38.F : 28.F, boss.id});
  }
  for (const auto& pickup : state.pickups) {
    result.entities.push_back({common::EntityKind::Pickup, pickup.position, 8.F, "pickup"});
  }
  result.hud.redHearts = state.redHearts;
  result.hud.shields = state.shields;
  result.hud.coins = state.coins;
  result.hud.bombs = state.bombs;
  result.hud.keys = state.keys;
  result.hud.activeItem = state.activeItem;
  result.hud.floor = state.floor;
  result.hud.roomState = state.roomCleared ? "Cleared" : "Combat";
  for (const auto& room : state.rooms) {
    if (room.revealed && (room.visited || room.current)) {
      result.minimap.push_back({room.type, room.gridX, room.gridY, room.current, room.cleared});
    }
  }
  return result;
}

}  // namespace isaac::viewmodel
