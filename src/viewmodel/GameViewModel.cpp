#include "viewmodel/GameViewModel.h"

#include <algorithm>
#include <sstream>
#include <string_view>

namespace {

isaac::presentation::CharacterStyle characterStyle(std::string_view id) {
  using Style = isaac::presentation::CharacterStyle;
  if (id == "magdalene") return Style::Magdalene;
  if (id == "cain") return Style::Cain;
  if (id == "judas") return Style::Judas;
  return Style::Isaac;
}

std::string roomName(isaac::common::RoomType type) {
  using isaac::common::RoomType;
  if (type == RoomType::Treasure) return "TREASURE";
  if (type == RoomType::Shop) return "SHOP";
  if (type == RoomType::Secret) return "SECRET";
  if (type == RoomType::Boss) return "BOSS";
  if (type == RoomType::Devil) return "DEVIL";
  return "NORMAL";
}

isaac::presentation::ObjectiveDTO buildObjective(const isaac::model::SessionSnapshot& state) {
  using isaac::common::RoomType;
  isaac::presentation::ObjectiveDTO objective;

  if (!state.roomCleared) {
    if (state.roomType == RoomType::Boss) {
      objective.title = state.bosses.size() > 1 ? "DEFEAT BOTH BOSSES" : "DEFEAT THE BOSS";
      objective.detail = "KEEP MOVING AND SHOOT WITH ARROW KEYS";
      objective.progress = "BOSSES LEFT " + std::to_string(state.bosses.size());
    } else {
      objective.title = "CLEAR THE ROOM";
      objective.detail = "DEFEAT EVERY ENEMY TO UNSEAL DOORS";
      objective.progress = "ENEMIES LEFT " + std::to_string(state.enemies.size());
    }
    objective.hint = "WASD DODGE - ARROW KEYS SHOOT";
    return objective;
  }

  if (state.roomType == RoomType::Boss) {
    objective.title = state.floor == 3 ? "COMPLETE THE RUN"
                                       : "DESCEND TO FLOOR " + std::to_string(state.floor + 1);
    objective.detail = state.floor == 3 ? "PRESS ENTER TO FINISH THE RUN"
                                        : "PRESS ENTER TO USE THE TRAPDOOR";
    objective.progress = state.floor == 3 ? "FINAL BOSS DEFEATED" : "FLOOR BOSS DEFEATED";
    objective.hint = state.devilRoomAvailable ? "OPTIONAL DEVIL ROOM IS NOW OPEN"
                                              : "TRAPDOOR READY IN THIS ROOM";
    return objective;
  }

  if (state.roomType == RoomType::Treasure) {
    objective.title = state.roomRewardCollected ? "TREASURE COLLECTED" : "CLAIM THE TREASURE";
    objective.detail = state.roomRewardCollected ? "SAD ONION INCREASED YOUR TEAR RATE"
                                                  : "PRESS ENTER TO OPEN THE CHEST";
    objective.progress = state.roomRewardCollected
                             ? "ROOM REWARD COMPLETE"
                             : "COST 1 KEY - YOU HAVE " + std::to_string(state.keys);
    objective.hint = "FOLLOW AN OPEN DOOR TO CONTINUE";
    return objective;
  }

  if (state.roomType == RoomType::Shop) {
    objective.title = state.roomRewardCollected ? "SHOP ITEM PURCHASED" : "BUY THE SHOP ITEM";
    objective.detail = state.roomRewardCollected ? "SMALL ROCK INCREASED YOUR DAMAGE"
                                                  : "PRESS ENTER TO BUY SMALL ROCK";
    objective.progress = state.roomRewardCollected
                             ? "ROOM REWARD COMPLETE"
                             : "COST 5 COINS - YOU HAVE " + std::to_string(state.coins);
    objective.hint = "DEFEATED ENEMIES MAY DROP COINS";
    return objective;
  }

  if (state.roomType == RoomType::Secret) {
    objective.title = "SECRET ROOM FOUND";
    objective.detail = state.roomRewardCollected ? "LUCKY TOE WAS COLLECTED"
                                                  : "ENTER THE ROOM TO CLAIM LUCKY TOE";
    objective.progress = state.roomRewardCollected ? "SECRET REWARD COMPLETE" : "REWARD WAITING";
    objective.hint = "RETURN THROUGH THE OPEN DOOR";
    return objective;
  }

  if (state.roomType == RoomType::Devil) {
    objective.title = "DEVIL ROOM DISCOVERED";
    objective.detail = "EXPLORE IT THEN RETURN TO THE BOSS ROOM";
    objective.progress = "OPTIONAL ROOM FOUND";
    objective.hint = "USE THE BOSS TRAPDOOR TO CONTINUE";
    return objective;
  }

  const auto revealedRooms = std::ranges::count_if(
      state.rooms, [](const isaac::model::RoomSnapshot& room) { return room.revealed; });
  const auto visitedRooms = std::ranges::count_if(state.rooms, [](const isaac::model::RoomSnapshot& room) {
    return room.revealed && (room.visited || room.current);
  });
  objective.title = "FIND THE FLOOR BOSS";
  objective.detail = "CLEAR ROOMS - GOLD DOORS USE 1 KEY";
  objective.progress = "ROOMS VISITED " + std::to_string(visitedRooms) + "/" +
                       std::to_string(revealedRooms);
  objective.hint = "PUSH A WALL AND PRESS E TO SEARCH";
  return objective;
}

}  // namespace

namespace isaac::viewmodel {

GameViewModel::GameViewModel(model::GameSessionInterface& session) : session_(session) {
  tickCommand_ = common::Command<float>([this](float seconds) { executeTick(seconds); });
  inputCommand_ = common::Command<presentation::RealtimeInput>(
      [this](presentation::RealtimeInput input) { realtimeInput_ = input; });
  actionCommand_ = common::Command<presentation::UserAction>(
      [this](presentation::UserAction action) { pendingActions_.push_back(action); });
  displayProperty_.set(buildDisplayState());
}

void GameViewModel::executeTick(float seconds) {
  const auto screenBefore = screen_;
  bool interact{};
  bool useBomb{};
  bool useActive{};

  for (const auto action : pendingActions_) {
    using presentation::UserAction;
    if (action == UserAction::NavigateUp && screen_ == common::ScreenState::MainMenu) {
      menuIndex_ = (menuIndex_ + 3) % 4;
    } else if (action == UserAction::NavigateDown && screen_ == common::ScreenState::MainMenu) {
      menuIndex_ = (menuIndex_ + 1) % 4;
    } else if (action == UserAction::NavigateLeft && screen_ == common::ScreenState::CharacterSelect) {
      const auto count = session_.selectableCharacterCount();
      selectedCharacter_ = (selectedCharacter_ + count - 1) % count;
    } else if (action == UserAction::NavigateRight && screen_ == common::ScreenState::CharacterSelect) {
      selectedCharacter_ = (selectedCharacter_ + 1) % session_.selectableCharacterCount();
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

  displayProperty_.set(buildDisplayState());
  for (const auto event : session_.drainEvents()) {
    if (event == model::ModelEvent::Shot) {
      presentationSignal_.emit(presentation::PresentationEvent::Shot);
    }
    if (event == model::ModelEvent::Hurt) {
      presentationSignal_.emit(presentation::PresentationEvent::Hurt);
    }
    if (event == model::ModelEvent::Pickup) {
      presentationSignal_.emit(presentation::PresentationEvent::Pickup);
    }
  }
  if (screenBefore != common::ScreenState::Defeat && screen_ == common::ScreenState::Defeat) {
    presentationSignal_.emit(presentation::PresentationEvent::Defeat);
  }
}

presentation::DisplayState GameViewModel::buildDisplayState() const {
  presentation::DisplayState result;
  result.screen = screen_;
  result.menuIndex = menuIndex_;
  result.movement = realtimeInput_.movement;
  result.shooting = realtimeInput_.shooting;
  const auto selected = session_.selectableCharacter(selectedCharacter_);
  result.selectionName = selected.displayName;
  result.selectionStyle = characterStyle(selected.id);
  std::ostringstream stats;
  stats << "HP " << selected.redHearts << " SPD " << static_cast<int>(selected.moveSpeed)
        << " DMG " << selected.damage << " LUCK " << selected.luck;
  result.selectionStats = stats.str();
  const auto& state = session_.snapshot();
  result.entities.push_back({common::EntityKind::Player, state.playerPosition, 18.F,
                             characterStyle(state.characterId)});
  for (const auto& projectile : state.projectiles) {
    result.entities.push_back({projectile.friendly ? common::EntityKind::PlayerProjectile
                                                   : common::EntityKind::EnemyProjectile,
                               projectile.position, 6.F});
  }
  for (const auto& enemy : state.enemies) {
    result.entities.push_back({common::EntityKind::Enemy, enemy.position, 16.F});
  }
  for (const auto& boss : state.bosses) {
    result.entities.push_back({common::EntityKind::Boss, boss.position,
                               boss.id == "moms_leg" ? 38.F : 28.F});
  }
  for (const auto& pickup : state.pickups) {
    result.entities.push_back({common::EntityKind::Pickup, pickup.position, 8.F});
  }
  result.hud.redHearts = state.redHearts;
  result.hud.shields = state.shields;
  result.hud.coins = state.coins;
  result.hud.bombs = state.bombs;
  result.hud.keys = state.keys;
  result.hud.activeItem = state.activeItem;
  result.hud.floor = state.floor;
  result.hud.moveSpeed = state.moveSpeed;
  result.hud.damage = state.damage;
  result.hud.shotsPerSecond = state.shotsPerSecond;
  result.hud.projectileSpeed = state.projectileSpeed;
  result.hud.elapsedSeconds = state.elapsedSeconds;
  result.hud.roomState = roomName(state.roomType) + (state.roomCleared ? " - CLEARED" : " - COMBAT");
  for (const auto& room : state.rooms) {
    if (room.revealed && (room.visited || room.current)) {
      result.minimap.push_back({room.type, room.gridX, room.gridY, room.current, room.cleared});
    }
  }
  for (const auto& door : state.doors) {
    if (!door.hidden) {
      result.doors.push_back({door.direction, door.targetType, door.locked, door.sealed});
    }
  }
  result.objective = buildObjective(state);
  result.trapdoorVisible = state.roomType == common::RoomType::Boss && state.roomCleared;
  return result;
}

}  // namespace isaac::viewmodel
