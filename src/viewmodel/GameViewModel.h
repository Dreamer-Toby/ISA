#pragma once

#include "model/GameSessionInterface.h"
#include "viewmodel/Binding.h"
#include "viewmodel/RenderDTO.h"

#include <vector>

namespace isaac::viewmodel {

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

struct GameProperties {
  Property<DisplayState> display;
};

struct GameCommands {
  Command<float> tick;
  Command<RealtimeInput> setInput;
  Command<UserAction> action;
};

struct GameSignals {
  Signal<PresentationEvent> presentation;
};

class GameViewModel {
 public:
  explicit GameViewModel(model::GameSessionInterface& session);
  GameViewModel(const GameViewModel&) = delete;
  GameViewModel& operator=(const GameViewModel&) = delete;

  [[nodiscard]] const GameProperties& properties() const { return properties_; }
  [[nodiscard]] const GameCommands& commands() const { return commands_; }
  [[nodiscard]] const GameSignals& signals() const { return signals_; }

 private:
  void executeTick(float seconds);
  [[nodiscard]] DisplayState buildDisplayState() const;

  model::GameSessionInterface& session_;
  common::ScreenState screen_{common::ScreenState::Start};
  std::size_t selectedCharacter_{};
  int menuIndex_{};
  RealtimeInput realtimeInput_;
  std::vector<UserAction> pendingActions_;
  GameProperties properties_;
  GameCommands commands_;
  GameSignals signals_;
};

}  // namespace isaac::viewmodel
