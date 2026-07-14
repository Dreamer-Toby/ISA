#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"

#include <string>
#include <vector>

namespace isaac::viewmodel {

enum class CharacterStyle { Isaac, Magdalene, Cain, Judas };

struct EntityDTO {
  common::EntityKind kind{};
  common::Vec2 position{};
  float radius{12.F};
  CharacterStyle characterStyle{CharacterStyle::Isaac};
};

struct HudDTO {
  int redHearts{3};
  int shields{};
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

struct DisplayState {
  common::ScreenState screen{common::ScreenState::Start};
  int menuIndex{};
  common::Vec2 movement;
  common::Vec2 shooting;
  std::string selectionName;
  std::string selectionStats;
  CharacterStyle selectionStyle{CharacterStyle::Isaac};
  std::vector<EntityDTO> entities;
  HudDTO hud;
  std::vector<MinimapRoomDTO> minimap;
};

}  // namespace isaac::viewmodel
