#include "view/GameView.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace isaac::view {

GameView::GameView(viewmodel::GameViewModel& viewModel, resource::ResourceManager& resources)
    : viewModel_(viewModel), resources_(resources),
      window_(sf::VideoMode({960U, 540U}), "ISA — MVVM course project") {
  window_.setVerticalSyncEnabled(true);
}

void GameView::pollEvents() {
  while (const auto event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    }
  }
}

viewmodel::InputCommand GameView::inputCommand() const {
  using Key = sf::Keyboard::Key;
  viewmodel::InputCommand command;
  command.movement = {
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::D)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::A)),
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::S)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::W))};
  command.shooting = {
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::Right)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::Left)),
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::Down)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::Up))};
  command.confirm = sf::Keyboard::isKeyPressed(Key::Enter);
  command.pause = sf::Keyboard::isKeyPressed(Key::Escape);
  command.useBomb = sf::Keyboard::isKeyPressed(Key::E);
  command.useActive = sf::Keyboard::isKeyPressed(Key::Space);
  return command;
}

void GameView::render() {
  const auto display = viewModel_.displayState();
  window_.clear(sf::Color(24, 18, 28));

  sf::RectangleShape room({880.F, 420.F});
  room.setPosition({40.F, 80.F});
  room.setFillColor(sf::Color(65, 48, 46));
  room.setOutlineThickness(5.F);
  room.setOutlineColor(sf::Color(119, 85, 68));
  window_.draw(room);

  for (const auto& entity : display.entities) {
    sf::CircleShape shape(entity.radius);
    shape.setOrigin({entity.radius, entity.radius});
    shape.setPosition({entity.position.x, entity.position.y});
    shape.setFillColor(entity.kind == common::EntityKind::Player ? sf::Color(220, 187, 166) : sf::Color(128, 48, 55));
    window_.draw(shape);
  }
  window_.display();
}

}  // namespace isaac::view

