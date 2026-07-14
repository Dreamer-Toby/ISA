#include "app/Application.h"

#include <SFML/System/Clock.hpp>

#include <algorithm>

namespace isaac::app {

Application::Application() : viewModel_(session_), view_(viewModel_, resources_) {}

int Application::run() {
  constexpr float fixedStep = 1.F / 60.F;
  sf::Clock clock;
  float accumulator = 0.F;
  while (view_.isOpen()) {
    view_.pollEvents();
    accumulator += std::min(clock.restart().asSeconds(), 0.25F);
    while (accumulator >= fixedStep) {
      viewModel_.commands().tick.execute(fixedStep);
      accumulator -= fixedStep;
    }
    view_.render();
  }
  return 0;
}

}  // namespace isaac::app
