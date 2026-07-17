#pragma once

#include "common/GameTypes.h"

#include <string_view>

namespace isaac::view {

struct EndingPresentation {
  bool usesDefeatTestament{};
  std::string_view title;
  std::string_view subtitle;
};

constexpr EndingPresentation endingPresentation(common::ScreenState screen) {
  if (screen == common::ScreenState::Defeat) return {true, "DEAR DIARY", {}};
  if (screen == common::ScreenState::Victory) {
    return {false, "VICTORY", "ALL BOSSES DEFEATED"};
  }
  return {};
}

}  // namespace isaac::view
