#pragma once

#include "common/MathTypes.h"

namespace isaac::viewmodel {

struct InputCommand {
  common::Vec2 movement;
  common::Vec2 shooting;
  bool confirm{};
  bool pause{};
  bool useBomb{};
  bool useActive{};
};

}  // namespace isaac::viewmodel

