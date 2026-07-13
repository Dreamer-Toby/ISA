#include "app/Application.h"

#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <utility>

namespace isaac::app {

Application::Application() : viewModel_(session_), view_(viewModel_, resources_) {}

void Application::prepareCharacterSelectEvidence() {
  prepareEvidence(EvidenceScreen::CharacterSelect, {});
}

void Application::prepareEvidence(EvidenceScreen screen, std::filesystem::path outputPath) {
  viewmodel::InputCommand confirm;
  confirm.confirm = true;
  const auto pressConfirm = [&] {
    viewModel_.update(1.F / 60.F, confirm);
    viewModel_.update(1.F / 60.F, {});
  };
  pressConfirm();
  if (screen != EvidenceScreen::MainMenu) pressConfirm();
  if (screen == EvidenceScreen::Gameplay || screen == EvidenceScreen::Paused) {
    pressConfirm();
    viewmodel::InputCommand enterCombat;
    enterCombat.movement = {1.F, 0.F};
    for (int frame = 0; frame < 210; ++frame) viewModel_.update(1.F / 60.F, enterCombat);
    viewmodel::InputCommand shoot;
    shoot.shooting = {1.F, 0.F};
    viewModel_.update(1.F / 60.F, shoot);
  }
  if (screen == EvidenceScreen::Paused) {
    viewmodel::InputCommand pause;
    pause.pause = true;
    viewModel_.update(1.F / 60.F, pause);
    viewModel_.update(1.F / 60.F, {});
  }
  evidenceMode_ = true;
  evidencePath_ = std::move(outputPath);
}

int Application::run() {
  constexpr float fixedStep = 1.F / 60.F;
  sf::Clock clock;
  float accumulator = 0.F;
  while (view_.isOpen()) {
    view_.pollEvents();
    accumulator += std::min(clock.restart().asSeconds(), 0.25F);
    const auto input = evidenceMode_ ? viewmodel::InputCommand{} : view_.inputCommand();
    while (accumulator >= fixedStep) {
      viewModel_.update(fixedStep, input);
      accumulator -= fixedStep;
    }
    view_.render();
    if (evidenceMode_ && !evidencePath_.empty() && ++evidenceFrames_ >= 30) {
      return view_.saveScreenshot(evidencePath_) ? 0 : 1;
    }
  }
  return 0;
}

}  // namespace isaac::app
