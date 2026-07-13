#pragma once

#include "resource/ResourceManager.h"
#include "viewmodel/GameViewModel.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace isaac::view {

class GameView {
 public:
  GameView(viewmodel::GameViewModel& viewModel, resource::ResourceManager& resources);

  [[nodiscard]] bool isOpen() const { return window_.isOpen(); }
  void pollEvents();
  [[nodiscard]] viewmodel::InputCommand inputCommand() const;
  void render();

 private:
  viewmodel::GameViewModel& viewModel_;
  resource::ResourceManager& resources_;
  sf::RenderWindow window_;
};

}  // namespace isaac::view

