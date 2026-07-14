#pragma once

#include "common/PresentationTypes.h"
#include "model/GameSessionInterface.h"
#include "viewmodel/Binding.h"

#include <vector>

namespace isaac::viewmodel {

class GameViewModel {
 public:
  explicit GameViewModel(model::GameSessionInterface& session);
  GameViewModel(const GameViewModel&) = delete;
  GameViewModel& operator=(const GameViewModel&) = delete;

  [[nodiscard]] const Property<presentation::DisplayState>& displayProperty() const {
    return displayProperty_;
  }
  [[nodiscard]] const Command<float>& tickCommand() const { return tickCommand_; }
  [[nodiscard]] const Command<presentation::RealtimeInput>& inputCommand() const {
    return inputCommand_;
  }
  [[nodiscard]] const Command<presentation::UserAction>& actionCommand() const {
    return actionCommand_;
  }
  [[nodiscard]] const Signal<presentation::PresentationEvent>& presentationSignal() const {
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
  Property<presentation::DisplayState> displayProperty_;
  Command<float> tickCommand_;
  Command<presentation::RealtimeInput> inputCommand_;
  Command<presentation::UserAction> actionCommand_;
  Signal<presentation::PresentationEvent> presentationSignal_;
};

}  // namespace isaac::viewmodel
