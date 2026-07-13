#pragma once

#include "resource/ResourceManager.h"
#include "viewmodel/GameViewModel.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>

namespace isaac::view {

class GameView {
 public:
  GameView(viewmodel::GameViewModel& viewModel, resource::ResourceManager& resources);

  [[nodiscard]] bool isOpen() const { return window_.isOpen(); }
  void pollEvents();
  [[nodiscard]] viewmodel::InputCommand inputCommand() const;
  void render();
  [[nodiscard]] bool saveScreenshot(const std::filesystem::path& path);

 private:
  viewmodel::GameViewModel& viewModel_;
  resource::ResourceManager& resources_;
  sf::RenderWindow window_;
  sf::Clock animationClock_;
  sf::Clock transitionClock_;
  sf::Clock damageFlashClock_;
  bool damageFlashActive_{};
  common::ScreenState previousScreen_{common::ScreenState::Start};
  int lastHearts_{-1};
  std::size_t lastProjectileCount_{};
  std::size_t lastPickupCount_{};
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
