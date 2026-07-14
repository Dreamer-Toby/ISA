#pragma once

#include "common/PresentationTypes.h"
#include "resource/ResourceManager.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace isaac::view {

class GameView {
 public:
  using ActionHandler = std::function<void(presentation::UserAction)>;
  using InputHandler = std::function<void(presentation::RealtimeInput)>;

  explicit GameView(resource::ResourceManager& resources);

  [[nodiscard]] bool isOpen() const { return window_.isOpen(); }
  void setActionHandler(ActionHandler handler) { actionHandler_ = std::move(handler); }
  void setInputHandler(InputHandler handler) { inputHandler_ = std::move(handler); }
  void setDisplay(const presentation::DisplayState& display);
  void present(presentation::PresentationEvent event);
  void pollEvents();
  void render();

 private:
  resource::ResourceManager& resources_;
  sf::RenderWindow window_;
  sf::Clock animationClock_;
  sf::Clock transitionClock_;
  sf::Clock damageFlashClock_;
  presentation::DisplayState display_;
  ActionHandler actionHandler_;
  InputHandler inputHandler_;
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
