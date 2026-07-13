#include "model/GameSession.h"
#include "viewmodel/GameViewModel.h"

#include <cstdlib>
#include <iostream>

namespace {
int failures{};
void check(bool condition, const char* message) {
  if (!condition) { std::cerr << "FAIL: " << message << '\n'; ++failures; }
}
void pressConfirm(isaac::viewmodel::GameViewModel& viewModel) {
  isaac::viewmodel::InputCommand command;
  command.confirm = true;
  viewModel.update(1.F / 60.F, command);
  viewModel.update(1.F / 60.F, {});
}
}  // namespace

int main() {
  using isaac::common::ScreenState;
  isaac::model::GameSession session;
  isaac::viewmodel::GameViewModel viewModel(session);
  check(viewModel.displayState().screen == ScreenState::Start, "starts at menu");
  pressConfirm(viewModel);
  check(viewModel.displayState().screen == ScreenState::CharacterSelect, "opens character select");
  pressConfirm(viewModel);
  check(viewModel.displayState().screen == ScreenState::Playing, "starts selected character");
  isaac::viewmodel::InputCommand pause;
  pause.pause = true;
  viewModel.update(1.F / 60.F, pause);
  check(viewModel.displayState().screen == ScreenState::Paused, "pause screen");
  viewModel.update(1.F / 60.F, {});
  viewModel.update(1.F / 60.F, pause);
  check(viewModel.displayState().screen == ScreenState::Playing, "resume screen");
  const auto hud = viewModel.displayState().hud;
  check(hud.redHearts > 0 && hud.floor == 1 && !hud.activeItem.empty(), "HUD exposes required state");
  session.player().resetInvulnerability();
  session.player().damage(20);
  viewModel.update(1.F / 60.F, {});
  check(viewModel.displayState().screen == ScreenState::Defeat, "death enters defeat screen");

  isaac::model::GameSession winningSession(0.99F);
  isaac::viewmodel::GameViewModel winningViewModel(winningSession);
  pressConfirm(winningViewModel);
  pressConfirm(winningViewModel);
  for (int expectedFloor = 1; expectedFloor <= 3; ++expectedFloor) {
    auto& level = winningSession.level();
    check(level.floorNumber() == expectedFloor, "expected floor in deterministic course flow");
    check(level.enter(1, winningSession.player().inventory(), false), "enter combat route");
    level.markCurrentCleared();
    check(level.enter(5, winningSession.player().inventory(), false), "enter boss route");
    level.markCurrentCleared();
    pressConfirm(winningViewModel);
  }
  check(winningViewModel.displayState().screen == ScreenState::Victory, "third Boss clear enters victory");
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
