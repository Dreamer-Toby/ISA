#pragma once

#include "resource/ResourceManager.h"
#include "viewmodel/GameViewModel.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <cstddef>
#include <memory>
#include <optional>

namespace isaac::view {

class GameView {
 public:
  GameView(viewmodel::GameViewModel& viewModel, resource::ResourceManager& resources);
  ~GameView();

  [[nodiscard]] bool isOpen() const { return window_.isOpen(); }
  void pollEvents();
  void render();

 private:
  void onDisplayChanged(const viewmodel::DisplayState& display);
  void onPresentationEvent(viewmodel::PresentationEvent event);

  viewmodel::GameViewModel& viewModel_;
  resource::ResourceManager& resources_;
  sf::RenderWindow window_;
  sf::Clock animationClock_;
  sf::Clock transitionClock_;
  sf::Clock damageFlashClock_;
  viewmodel::DisplayState display_;
  std::size_t displayConnection_{};
  std::size_t presentationConnection_{};
  bool damageFlashActive_{};
  bool showHitboxes_{};
  std::shared_ptr<sf::SoundBuffer> shootBuffer_;
  std::shared_ptr<sf::SoundBuffer> hurtBuffer_;
  std::shared_ptr<sf::SoundBuffer> pickupBuffer_;
  std::shared_ptr<sf::SoundBuffer> defeatBuffer_;
  std::optional<sf::Sound> shootSound_;
  std::optional<sf::Sound> hurtSound_;
  std::optional<sf::Sound> pickupSound_;
  std::optional<sf::Sound> defeatSound_;
};

}  // namespace isaac::view
