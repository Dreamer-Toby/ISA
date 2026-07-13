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
    if (entity.kind == common::EntityKind::Player) shape.setFillColor(sf::Color(220, 187, 166));
    else if (entity.kind == common::EntityKind::PlayerProjectile) shape.setFillColor(sf::Color(150, 205, 235));
    else if (entity.kind == common::EntityKind::Pickup) shape.setFillColor(sf::Color(225, 190, 70));
    else shape.setFillColor(sf::Color(128, 48, 55));
    window_.draw(shape);
  }

  for (int i = 0; i < display.hud.redHearts; ++i) {
    sf::CircleShape heart(7.F);
    heart.setPosition({18.F + 17.F * static_cast<float>(i), 18.F});
    heart.setFillColor(sf::Color(205, 45, 62));
    window_.draw(heart);
  }
  for (int i = 0; i < display.hud.shields; ++i) {
    sf::CircleShape shield(7.F);
    shield.setPosition({18.F + 17.F * static_cast<float>(display.hud.redHearts + i), 18.F});
    shield.setFillColor(sf::Color(120, 180, 210));
    window_.draw(shield);
  }
  for (const auto& roomState : display.minimap) {
    sf::RectangleShape cell({12.F, 8.F});
    cell.setPosition({820.F + 16.F * static_cast<float>(roomState.x),
                      34.F + 12.F * static_cast<float>(roomState.y)});
    cell.setFillColor(roomState.current ? sf::Color(235, 210, 120) : sf::Color(125, 115, 110));
    window_.draw(cell);
  }
  window_.display();
}

}  // namespace isaac::view
