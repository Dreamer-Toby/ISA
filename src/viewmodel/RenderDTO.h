#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"

#include <string>
#include <vector>

namespace isaac::viewmodel {

struct EntityDTO {
  common::EntityKind kind{};
  common::Vec2 position{};
  float radius{12.F};
  std::string styleId;
};

struct HudDTO {
  int redHearts{3};
  int shields{};
  int coins{};
  int bombs{1};
  int keys{1};
  int floor{1};
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
  std::vector<EntityDTO> entities;
  HudDTO hud;
  std::vector<MinimapRoomDTO> minimap;
};

}  // namespace isaac::viewmodel
