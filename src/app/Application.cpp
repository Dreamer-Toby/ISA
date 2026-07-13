#include "app/Application.h"

#include <SFML/System/Clock.hpp>

#include <algorithm>

namespace isaac::app {

Application::Application() : viewModel_(session_), view_(viewModel_, resources_) {}

void Application::prepareCharacterSelectEvidence() {
  viewmodel::InputCommand confirm;
  confirm.confirm = true;
  viewModel_.update(1.F / 60.F, confirm);
  viewModel_.update(1.F / 60.F, {});
  evidenceMode_ = true;
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
  }
  return 0;
}

}  // namespace isaac::app
