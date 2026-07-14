#include "viewmodel/GameViewModel.h"

#include "model/CharacterCatalog.h"

#include <sstream>

namespace isaac::viewmodel {

GameViewModel::GameViewModel(model::GameSession& session) : session_(session) {
  commands_.tick = Command<float>([this](float seconds) { executeTick(seconds); });
  commands_.setInput = Command<RealtimeInput>([this](RealtimeInput input) { realtimeInput_ = input; });
  commands_.action = Command<UserAction>([this](UserAction action) { pendingActions_.push_back(action); });
  properties_.display.set(buildDisplayState());
}

void GameViewModel::executeTick(float seconds) {
  const auto& before = session_.snapshot();
  const auto shotsBefore = before.totalShots;
  const auto heartsBefore = before.redHearts + before.shields;
  const auto pickupsBefore = before.pickups.size();
  const auto screenBefore = screen_;
  bool interact{};
  bool useBomb{};
  bool useActive{};

  for (const auto action : pendingActions_) {
    if (action == UserAction::NavigateUp && screen_ == common::ScreenState::MainMenu) {
      menuIndex_ = (menuIndex_ + 3) % 4;
    } else if (action == UserAction::NavigateDown && screen_ == common::ScreenState::MainMenu) {
      menuIndex_ = (menuIndex_ + 1) % 4;
    } else if (action == UserAction::NavigateLeft && screen_ == common::ScreenState::CharacterSelect) {
      selectedCharacter_ = (selectedCharacter_ + 3) % 4;
    } else if (action == UserAction::NavigateRight && screen_ == common::ScreenState::CharacterSelect) {
      selectedCharacter_ = (selectedCharacter_ + 1) % 4;
    } else if (action == UserAction::Confirm) {
      if (screen_ == common::ScreenState::Start) {
        screen_ = common::ScreenState::MainMenu;
      } else if (screen_ == common::ScreenState::MainMenu) {
        if (menuIndex_ == 0) screen_ = common::ScreenState::CharacterSelect;
        if (menuIndex_ == 1) {
          selectedCharacter_ = 0;
          session_.selectCharacter(selectedCharacter_);
          screen_ = common::ScreenState::Playing;
        }
        if (menuIndex_ == 2) screen_ = common::ScreenState::Rankings;
        if (menuIndex_ == 3) screen_ = common::ScreenState::Start;
      } else if (screen_ == common::ScreenState::Rankings) {
        screen_ = common::ScreenState::MainMenu;
      } else if (screen_ == common::ScreenState::CharacterSelect) {
        session_.selectCharacter(selectedCharacter_);
        screen_ = common::ScreenState::Playing;
      } else if (screen_ == common::ScreenState::Playing) {
        interact = true;
      } else if (screen_ == common::ScreenState::Defeat || screen_ == common::ScreenState::Victory) {
        screen_ = common::ScreenState::MainMenu;
      }
    } else if (action == UserAction::Back) {
      if (screen_ == common::ScreenState::MainMenu) screen_ = common::ScreenState::Start;
      else if (screen_ == common::ScreenState::Rankings ||
               screen_ == common::ScreenState::CharacterSelect) screen_ = common::ScreenState::MainMenu;
      else if (screen_ == common::ScreenState::Playing) screen_ = common::ScreenState::Paused;
      else if (screen_ == common::ScreenState::Paused) screen_ = common::ScreenState::Playing;
    } else if (action == UserAction::UseBomb && screen_ == common::ScreenState::Playing) {
      useBomb = true;
    } else if (action == UserAction::UseActive && screen_ == common::ScreenState::Playing) {
      useActive = true;
    }
  }
  pendingActions_.clear();

  if (screen_ == common::ScreenState::Playing) {
    session_.update(seconds, model::GameplayInput{realtimeInput_.movement, realtimeInput_.shooting,
                                                  useBomb, useActive, interact});
  }

  const auto& after = session_.snapshot();
  if (screen_ == common::ScreenState::Playing && after.playerDead) screen_ = common::ScreenState::Defeat;
  if (screen_ == common::ScreenState::Playing && after.runCompleted) screen_ = common::ScreenState::Victory;

  properties_.display.set(buildDisplayState());
  if (after.totalShots > shotsBefore) signals_.presentation.emit(PresentationEvent::Shot);
  if (after.redHearts + after.shields < heartsBefore) signals_.presentation.emit(PresentationEvent::Hurt);
  if (pickupsBefore > 0 && after.pickups.size() < pickupsBefore) {
    signals_.presentation.emit(PresentationEvent::Pickup);
  }
  if (screenBefore != common::ScreenState::Defeat && screen_ == common::ScreenState::Defeat) {
    signals_.presentation.emit(PresentationEvent::Defeat);
  }
}

DisplayState GameViewModel::buildDisplayState() const {
  DisplayState result;
  result.screen = screen_;
  result.menuIndex = menuIndex_;
  result.movement = realtimeInput_.movement;
  result.shooting = realtimeInput_.shooting;
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
  result.hud.moveSpeed = session_.player().definition().moveSpeed;
  result.hud.damage = session_.player().shooting().damage();
  result.hud.shotsPerSecond = session_.player().definition().shotsPerSecond;
  result.hud.projectileSpeed = session_.player().shooting().projectileSpeed();
  result.hud.elapsedSeconds = state.elapsedSeconds;
  result.hud.roomState = state.roomCleared ? "Cleared" : "Combat";
  for (const auto& room : state.rooms) {
    if (room.revealed && (room.visited || room.current)) {
      result.minimap.push_back({room.type, room.gridX, room.gridY, room.current, room.cleared});
    }
  }
  return result;
}

}  // namespace isaac::viewmodel
