#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"

#include <string>
#include <vector>

namespace isaac::presentation {

struct RealtimeInput {
  common::Vec2 movement;
  common::Vec2 shooting;
};

enum class UserAction {
  Confirm,
  Back,
  NavigateUp,
  NavigateDown,
  NavigateLeft,
  NavigateRight,
  UseBomb,
  UseActive
};

enum class PresentationEvent { Shot, Hurt, Pickup, Defeat };

enum class CharacterStyle { Isaac, Magdalene, Cain, Judas };

struct EntityDTO {
  common::EntityKind kind{};
  common::Vec2 position{};
  float radius{12.F};
  CharacterStyle characterStyle{CharacterStyle::Isaac};
  std::string itemId;
};

struct HudDTO {
  int heartContainers{3};
  int redHearts{3};
  bool redHalfHeart{};
  int shields{};
  bool shieldHalfHeart{};
  int coins{};
  int bombs{1};
  int keys{1};
  int floor{1};
  float moveSpeed{};
  float damage{};
  float shotsPerSecond{};
  float projectileSpeed{};
  float elapsedSeconds{};
  std::string activeItem{"None"};
  std::string roomState{"Start room"};
};

struct MinimapRoomDTO {
  common::RoomType type{};
  int x{};
  int y{};
  bool current{};
  bool cleared{};
};

struct DoorDTO {
  common::Direction direction{common::Direction::Up};
  common::RoomType targetType{common::RoomType::Normal};
  bool locked{};
  bool sealed{};
};

struct DisplayState {
  common::ScreenState screen{common::ScreenState::Start};
  int menuIndex{};
  int pauseMenuIndex{};
  common::Vec2 movement;
  common::Vec2 shooting;
  std::string selectionName;
  std::string selectionStats;
  CharacterStyle selectionStyle{CharacterStyle::Isaac};
  std::vector<EntityDTO> entities;
  HudDTO hud;
  std::vector<MinimapRoomDTO> minimap;
  std::vector<DoorDTO> doors;
  bool trapdoorVisible{};
};

}  // namespace isaac::presentation
