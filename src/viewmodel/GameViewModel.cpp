#include "viewmodel/GameViewModel.h"

namespace isaac::viewmodel {

void GameViewModel::update(float seconds, const InputCommand& command) {
  const bool confirmPressed = command.confirm && !confirmWasDown_;
  const bool pausePressed = command.pause && !pauseWasDown_;
  confirmWasDown_ = command.confirm;
  pauseWasDown_ = command.pause;

  if (screen_ == common::ScreenState::Start && confirmPressed) {
    screen_ = common::ScreenState::CharacterSelect;
  } else if (screen_ == common::ScreenState::CharacterSelect && confirmPressed) {
    screen_ = common::ScreenState::Playing;
  } else if (screen_ == common::ScreenState::Playing && pausePressed) {
    screen_ = common::ScreenState::Paused;
  } else if (screen_ == common::ScreenState::Paused && pausePressed) {
    screen_ = common::ScreenState::Playing;
  }

  if (screen_ == common::ScreenState::Playing) {
    session_.update(seconds, model::GameplayInput{command.movement, command.shooting});
  }
}

DisplayState GameViewModel::displayState() const {
  DisplayState result;
  result.screen = screen_;
  const auto& state = session_.snapshot();
  result.entities.push_back({common::EntityKind::Player, state.playerPosition, 18.F, "isaac"});
  return result;
}

}  // namespace isaac::viewmodel

