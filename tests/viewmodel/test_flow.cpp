#include "model/GameSession.h"
#include "viewmodel/GameViewModel.h"

#include <cstdlib>
#include <iostream>

namespace {
constexpr float Step = 1.F / 60.F;
int failures{};

void check(bool condition, const char* message) {
  if (!condition) { std::cerr << "FAIL: " << message << '\n'; ++failures; }
}

void tick(isaac::viewmodel::GameViewModel& viewModel) {
  viewModel.tickCommand().execute(Step);
}

void act(isaac::viewmodel::GameViewModel& viewModel, isaac::presentation::UserAction action) {
  viewModel.actionCommand().execute(action);
  tick(viewModel);
}

const isaac::presentation::DisplayState& display(const isaac::viewmodel::GameViewModel& viewModel) {
  return viewModel.displayProperty().get();
}
}  // namespace

int main() {
  using isaac::common::ScreenState;
  using isaac::presentation::UserAction;

  {
    isaac::model::GameSession bindingSession;
    isaac::viewmodel::GameViewModel bindingViewModel(bindingSession);
    int propertyChanges{};
    const auto connection = bindingViewModel.displayProperty().changed().connect(
        [&](const isaac::presentation::DisplayState&) { ++propertyChanges; });
    bindingViewModel.actionCommand().execute(UserAction::Confirm);
    tick(bindingViewModel);
    check(display(bindingViewModel).screen == ScreenState::MainMenu,
          "confirm command publishes the display property");
    check(propertyChanges == 1, "display property notifies its bound view once");
    bindingViewModel.displayProperty().changed().disconnect(connection);
    tick(bindingViewModel);
    check(propertyChanges == 1, "disconnected view no longer receives property changes");
  }

  {
    isaac::model::GameSession signalSession;
    isaac::viewmodel::GameViewModel signalViewModel(signalSession);
    int shotSignals{};
    const auto connection = signalViewModel.presentationSignal().connect(
        [&](isaac::presentation::PresentationEvent event) {
          if (event == isaac::presentation::PresentationEvent::Shot) ++shotSignals;
        });
    for (int step = 0; step < 3; ++step) act(signalViewModel, UserAction::Confirm);
    signalViewModel.inputCommand().execute({{}, {1.F, 0.F}});
    tick(signalViewModel);
    check(shotSignals == 1, "shoot command emits one presentation signal");
    signalViewModel.presentationSignal().disconnect(connection);
  }

  {
    isaac::model::GameSession quickSession;
    isaac::viewmodel::GameViewModel quickViewModel(quickSession);
    act(quickViewModel, UserAction::Confirm);
    act(quickViewModel, UserAction::NavigateDown);
    act(quickViewModel, UserAction::Confirm);
    check(display(quickViewModel).screen == ScreenState::Playing,
          "quick run starts Isaac without character selection");
    check(quickSession.snapshot().characterId == "isaac", "quick run selects Isaac");
  }

  {
    isaac::model::GameSession rankSession;
    isaac::viewmodel::GameViewModel rankViewModel(rankSession);
    act(rankViewModel, UserAction::Confirm);
    act(rankViewModel, UserAction::NavigateDown);
    act(rankViewModel, UserAction::NavigateDown);
    act(rankViewModel, UserAction::Confirm);
    check(display(rankViewModel).screen == ScreenState::Rankings, "rankings page opens");
    act(rankViewModel, UserAction::Back);
    check(display(rankViewModel).screen == ScreenState::MainMenu, "rankings page returns to menu");
  }

  isaac::model::GameSession session;
  isaac::viewmodel::GameViewModel viewModel(session);
  check(display(viewModel).screen == ScreenState::Start, "starts at menu");
  act(viewModel, UserAction::Confirm);
  check(display(viewModel).screen == ScreenState::MainMenu, "opens paper main menu");
  act(viewModel, UserAction::Confirm);
  check(display(viewModel).screen == ScreenState::CharacterSelect, "opens character select");
  act(viewModel, UserAction::NavigateRight);
  act(viewModel, UserAction::NavigateRight);
  check(display(viewModel).selectionName == "Cain" &&
        display(viewModel).selectionStats.find("SPD") != std::string::npos,
        "non-default character and stats are visible");
  act(viewModel, UserAction::Confirm);
  check(display(viewModel).screen == ScreenState::Playing, "starts selected character");
  check(session.snapshot().characterId == "cain", "non-default selected character reaches Model");
  act(viewModel, UserAction::Back);
  check(display(viewModel).screen == ScreenState::Paused, "pause screen");
  act(viewModel, UserAction::Back);
  check(display(viewModel).screen == ScreenState::Playing, "resume screen");
  const auto hud = display(viewModel).hud;
  check(hud.redHearts > 0 && hud.floor == 1 && !hud.activeItem.empty(), "HUD exposes required state");
  session.player().resetInvulnerability();
  session.player().damage(20);
  tick(viewModel);
  check(display(viewModel).screen == ScreenState::Defeat, "death enters defeat screen");

  isaac::model::GameSession winningSession(0.99F);
  isaac::viewmodel::GameViewModel winningViewModel(winningSession);
  act(winningViewModel, UserAction::Confirm);
  act(winningViewModel, UserAction::Confirm);
  act(winningViewModel, UserAction::Confirm);
  for (int expectedFloor = 1; expectedFloor <= 3; ++expectedFloor) {
    auto& level = winningSession.level();
    check(level.floorNumber() == expectedFloor, "expected floor in deterministic course flow");
    check(level.enter(1, winningSession.player().inventory(), false), "enter combat route");
    level.markCurrentCleared();
    check(level.enter(5, winningSession.player().inventory(), false), "enter boss route");
    level.markCurrentCleared();
    act(winningViewModel, UserAction::Confirm);
  }
  check(display(winningViewModel).screen == ScreenState::Victory, "third Boss clear enters victory");
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
