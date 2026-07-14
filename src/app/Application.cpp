#include "app/Application.h"

#include <SFML/System/Clock.hpp>

#include <algorithm>

namespace isaac::app {

Application::Application() : viewModel_(session_), view_(resources_) {
  view_.setDisplay(viewModel_.displayProperty().get());

  displayConnection_ = viewModel_.displayProperty().changed().connect(
      [this](const presentation::DisplayState& display) { view_.setDisplay(display); });
  view_.setActionHandler([this](presentation::UserAction action) {
    viewModel_.actionCommand().execute(action);
  });
  view_.setInputHandler([this](presentation::RealtimeInput input) {
    viewModel_.inputCommand().execute(input);
  });
  presentationConnection_ = viewModel_.presentationSignal().connect(
      [this](presentation::PresentationEvent event) { view_.present(event); });
}

Application::~Application() {
  viewModel_.displayProperty().changed().disconnect(displayConnection_);
  viewModel_.presentationSignal().disconnect(presentationConnection_);
}

int Application::run() {
  constexpr float fixedStep = 1.F / 60.F;
  sf::Clock clock;
  float accumulator = 0.F;
  while (view_.isOpen()) {
    view_.pollEvents();
    accumulator += std::min(clock.restart().asSeconds(), 0.25F);
    while (accumulator >= fixedStep) {
      viewModel_.tickCommand().execute(fixedStep);
      accumulator -= fixedStep;
    }
    view_.render();
  }
  return 0;
}

}  // namespace isaac::app
