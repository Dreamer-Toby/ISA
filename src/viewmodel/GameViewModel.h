#pragma once

#include "common/Binding.h"
#include "common/PresentationTypes.h"
#include "model/GameSessionInterface.h"

#include <vector>

namespace isaac::viewmodel {

class GameViewModel {
 public:
  explicit GameViewModel(model::GameSessionInterface& session);
  GameViewModel(const GameViewModel&) = delete;
  GameViewModel& operator=(const GameViewModel&) = delete;

  [[nodiscard]] const common::Property<presentation::DisplayState>& displayProperty() const {
    return displayProperty_;
  }
  [[nodiscard]] const common::Command<float>& tickCommand() const { return tickCommand_; }
  [[nodiscard]] const common::Command<presentation::RealtimeInput>& inputCommand() const {
    return inputCommand_;
  }
  [[nodiscard]] const common::Command<presentation::UserAction>& actionCommand() const {
    return actionCommand_;
  }
  [[nodiscard]] const common::Signal<presentation::PresentationEvent>& presentationSignal() const {
    return presentationSignal_;
  }

 private:
  void executeTick(float seconds);
  [[nodiscard]] presentation::DisplayState buildDisplayState() const;

  model::GameSessionInterface& session_;
  common::ScreenState screen_{common::ScreenState::Start};
  std::size_t selectedCharacter_{};
  int menuIndex_{};
  presentation::RealtimeInput realtimeInput_;
  std::vector<presentation::UserAction> pendingActions_;
  common::Property<presentation::DisplayState> displayProperty_;
  common::Command<float> tickCommand_;
  common::Command<presentation::RealtimeInput> inputCommand_;
  common::Command<presentation::UserAction> actionCommand_;
  common::Signal<presentation::PresentationEvent> presentationSignal_;
};

}  // namespace isaac::viewmodel
