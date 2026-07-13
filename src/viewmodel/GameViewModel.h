#pragma once

#include "model/GameSession.h"
#include "viewmodel/InputCommand.h"
#include "viewmodel/RenderDTO.h"

namespace isaac::viewmodel {

class GameViewModel {
 public:
  explicit GameViewModel(model::GameSession& session) : session_(session) {}

  void update(float seconds, const InputCommand& command);
  [[nodiscard]] DisplayState displayState() const;

 private:
  model::GameSession& session_;
  common::ScreenState screen_{common::ScreenState::Start};
  std::size_t selectedCharacter_{};
  int menuIndex_{};
  bool confirmWasDown_{};
  bool pauseWasDown_{};
  int horizontalWasDown_{};
  int verticalWasDown_{};
};

}  // namespace isaac::viewmodel
