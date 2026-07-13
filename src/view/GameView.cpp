#include "view/GameView.h"
#include "resource/AssetCatalog.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <array>
#include <cctype>
#include <string_view>
#include <unordered_map>

namespace {
using Glyph = std::array<unsigned char, 7>;
const Glyph& glyph(char value) {
  static const std::unordered_map<char, Glyph> font{
      {'A',{14,17,17,31,17,17,17}}, {'B',{30,17,17,30,17,17,30}}, {'C',{14,17,16,16,16,17,14}},
      {'D',{30,17,17,17,17,17,30}}, {'E',{31,16,16,30,16,16,31}}, {'F',{31,16,16,30,16,16,16}},
      {'G',{14,17,16,23,17,17,15}}, {'H',{17,17,17,31,17,17,17}}, {'I',{31,4,4,4,4,4,31}},
      {'J',{7,2,2,2,18,18,12}}, {'K',{17,18,20,24,20,18,17}}, {'L',{16,16,16,16,16,16,31}},
      {'M',{17,27,21,21,17,17,17}}, {'N',{17,25,21,19,17,17,17}}, {'O',{14,17,17,17,17,17,14}},
      {'P',{30,17,17,30,16,16,16}}, {'Q',{14,17,17,17,21,18,13}}, {'R',{30,17,17,30,20,18,17}},
      {'S',{15,16,16,14,1,1,30}}, {'T',{31,4,4,4,4,4,4}}, {'U',{17,17,17,17,17,17,14}},
      {'V',{17,17,17,17,17,10,4}}, {'W',{17,17,17,21,21,21,10}}, {'X',{17,17,10,4,10,17,17}},
      {'Y',{17,17,10,4,4,4,4}}, {'Z',{31,1,2,4,8,16,31}},
      {'0',{14,17,19,21,25,17,14}}, {'1',{4,12,4,4,4,4,14}}, {'2',{14,17,1,2,4,8,31}},
      {'3',{30,1,1,14,1,1,30}}, {'4',{2,6,10,18,31,2,2}}, {'5',{31,16,16,30,1,1,30}},
      {'6',{14,16,16,30,17,17,14}}, {'7',{31,1,2,4,8,8,8}}, {'8',{14,17,17,14,17,17,14}},
      {'9',{14,17,17,15,1,1,14}}, {'-',{0,0,0,31,0,0,0}}, {'.',{0,0,0,0,0,12,12}},
  };
  static const Glyph blank{};
  const auto found = font.find(static_cast<char>(std::toupper(static_cast<unsigned char>(value))));
  return found == font.end() ? blank : found->second;
}

void drawBitmapText(sf::RenderWindow& window, std::string_view text, sf::Vector2f position,
                    float scale = 2.F, sf::Color color = sf::Color(238, 227, 209)) {
  float x = position.x;
  for (char character : text) {
    if (character == ' ') { x += 4.F * scale; continue; }
    const auto& rows = glyph(character);
    for (std::size_t row = 0; row < rows.size(); ++row) {
      for (int column = 0; column < 5; ++column) {
        if ((rows[row] & (1U << (4 - column))) == 0) continue;
        sf::RectangleShape pixel({scale, scale});
        pixel.setPosition({x + static_cast<float>(column) * scale,
                           position.y + static_cast<float>(row) * scale});
        pixel.setFillColor(color);
        window.draw(pixel);
      }
    }
    x += 6.F * scale;
  }
}
}  // namespace

namespace isaac::view {

GameView::GameView(viewmodel::GameViewModel& viewModel, resource::ResourceManager& resources)
    : viewModel_(viewModel), resources_(resources),
      window_(sf::VideoMode({960U, 540U}), "ISA - MVVM course project") {
  window_.setVerticalSyncEnabled(true);
  window_.setPosition({100, 100});
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
  if (display.screen == common::ScreenState::Start) window_.setTitle("ISA - Press Enter to start");
  else if (display.screen == common::ScreenState::CharacterSelect) window_.setTitle("ISA - Character select: A/D then Enter");
  else if (display.screen == common::ScreenState::Defeat) window_.setTitle("ISA - Defeat");
  else if (display.screen == common::ScreenState::Victory) window_.setTitle("ISA - Victory");
  else {
    std::ostringstream title;
    title << "ISA | Floor " << display.hud.floor << " | Coins " << display.hud.coins
          << " | Bombs " << display.hud.bombs << " | Keys " << display.hud.keys
          << " | Active " << display.hud.activeItem << " | " << display.hud.roomState;
    if (display.screen == common::ScreenState::Paused) title << " | PAUSED";
    window_.setTitle(title.str());
  }
  window_.clear(sf::Color(24, 18, 28));

  if (display.screen == common::ScreenState::Start ||
      display.screen == common::ScreenState::CharacterSelect ||
      display.screen == common::ScreenState::Defeat ||
      display.screen == common::ScreenState::Victory) {
    sf::RectangleShape panel({520.F, 260.F});
    panel.setPosition({220.F, 140.F});
    if (display.screen == common::ScreenState::Start) panel.setFillColor(sf::Color(88, 57, 70));
    if (display.screen == common::ScreenState::CharacterSelect) panel.setFillColor(sf::Color(85, 74, 48));
    if (display.screen == common::ScreenState::Defeat) panel.setFillColor(sf::Color(92, 35, 42));
    if (display.screen == common::ScreenState::Victory) panel.setFillColor(sf::Color(48, 91, 76));
    panel.setOutlineThickness(6.F);
    panel.setOutlineColor(sf::Color(210, 185, 145));
    window_.draw(panel);
    if (display.screen == common::ScreenState::Start) drawBitmapText(window_, "ISA  PRESS ENTER", {300.F, 250.F}, 4.F);
    if (display.screen == common::ScreenState::CharacterSelect) {
      drawBitmapText(window_, "CHOOSE " + display.selectionName, {285.F, 210.F}, 3.F);
      drawBitmapText(window_, display.selectionStats, {265.F, 285.F}, 2.F);
    }
    if (display.screen == common::ScreenState::Defeat) drawBitmapText(window_, "DEFEAT", {365.F, 250.F}, 5.F);
    if (display.screen == common::ScreenState::Victory) drawBitmapText(window_, "VICTORY", {340.F, 250.F}, 5.F);
    window_.display();
    return;
  }

  sf::RectangleShape room({880.F, 420.F});
  room.setPosition({40.F, 80.F});
  room.setFillColor(sf::Color(65, 48, 46));
  room.setOutlineThickness(5.F);
  room.setOutlineColor(sf::Color(119, 85, 68));
  window_.draw(room);

  if (const auto doorTexture = resources_.texture(resource::AssetCatalog::door())) {
    sf::Sprite door(*doorTexture);
    door.setPosition({871.F, 274.F});
    door.setScale({1.5F, 1.5F});
    window_.draw(door);
  }

  for (const auto& entity : display.entities) {
    std::filesystem::path texturePath;
    if (entity.kind == common::EntityKind::Player && entity.styleId == "isaac") texturePath = resource::AssetCatalog::isaac();
    if (entity.kind == common::EntityKind::Player && entity.styleId == "magdalene") texturePath = resource::AssetCatalog::magdalene();
    if (entity.kind == common::EntityKind::Player && entity.styleId == "cain") texturePath = resource::AssetCatalog::cain();
    if (entity.kind == common::EntityKind::Player && entity.styleId == "judas") texturePath = resource::AssetCatalog::judas();
    if (entity.kind == common::EntityKind::Enemy && entity.styleId == "fly") texturePath = resource::AssetCatalog::fly();
    if (entity.kind == common::EntityKind::Boss) texturePath = resource::AssetCatalog::boss();
    if (entity.kind == common::EntityKind::PlayerProjectile || entity.kind == common::EntityKind::EnemyProjectile)
      texturePath = resource::AssetCatalog::tear();
    if (!texturePath.empty()) {
      if (const auto texture = resources_.texture(texturePath)) {
        sf::Sprite sprite(*texture);
        const auto size = texture->getSize();
        sprite.setOrigin({static_cast<float>(size.x) / 2.F, static_cast<float>(size.y) / 2.F});
        const float scale = (entity.radius * 2.F) / static_cast<float>(std::max(size.x, size.y));
        sprite.setScale({scale, scale});
        sprite.setPosition({entity.position.x, entity.position.y});
        window_.draw(sprite);
        continue;
      }
    }
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
    if (const auto texture = resources_.texture(resource::AssetCatalog::redHeart())) {
      sf::Sprite heart(*texture);
      heart.setPosition({18.F + 18.F * static_cast<float>(i), 18.F});
      window_.draw(heart);
      continue;
    }
    sf::CircleShape heart(7.F);
    heart.setPosition({18.F + 17.F * static_cast<float>(i), 18.F});
    heart.setFillColor(sf::Color(205, 45, 62));
    window_.draw(heart);
  }
  for (int i = 0; i < display.hud.shields; ++i) {
    if (const auto texture = resources_.texture(resource::AssetCatalog::shieldHeart())) {
      sf::Sprite shield(*texture);
      shield.setPosition({18.F + 18.F * static_cast<float>(display.hud.redHearts + i), 18.F});
      window_.draw(shield);
      continue;
    }
    sf::CircleShape shield(7.F);
    shield.setPosition({18.F + 17.F * static_cast<float>(display.hud.redHearts + i), 18.F});
    shield.setFillColor(sf::Color(120, 180, 210));
    window_.draw(shield);
  }
  std::ostringstream counters;
  counters << "COINS " << display.hud.coins << "  BOMBS " << display.hud.bombs
           << "  KEYS " << display.hud.keys << "  FLOOR " << display.hud.floor;
  drawBitmapText(window_, counters.str(), {180.F, 12.F}, 2.F);
  drawBitmapText(window_, "ACTIVE " + display.hud.activeItem + "  " + display.hud.roomState,
                 {180.F, 38.F}, 1.5F, sf::Color(205, 192, 170));
  for (const auto& roomState : display.minimap) {
    sf::RectangleShape cell({12.F, 8.F});
    cell.setPosition({820.F + 16.F * static_cast<float>(roomState.x),
                      34.F + 12.F * static_cast<float>(roomState.y)});
    cell.setFillColor(roomState.current ? sf::Color(235, 210, 120) : sf::Color(125, 115, 110));
    window_.draw(cell);
  }
  if (display.screen == common::ScreenState::Paused) {
    sf::RectangleShape paused({960.F, 540.F});
    paused.setFillColor(sf::Color(12, 10, 15, 155));
    window_.draw(paused);
  }
  window_.display();
}

}  // namespace isaac::view
