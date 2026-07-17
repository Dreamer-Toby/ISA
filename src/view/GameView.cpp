#include "view/GameView.h"

#include "resource/AssetCatalog.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <sstream>
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
      {':',{0,12,12,0,12,12,0}}, {'/',{1,2,2,4,8,8,16}}, {'<',{2,4,8,16,8,4,2}},
      {'>',{8,4,2,1,2,4,8}}, {'?',{14,17,1,2,4,0,4}},
  };
  static const Glyph blank{};
  const auto found = font.find(static_cast<char>(std::toupper(static_cast<unsigned char>(value))));
  return found == font.end() ? blank : found->second;
}

void drawBitmapText(sf::RenderWindow& window, std::string_view text, sf::Vector2f position,
                    float scale, sf::Color color) {
  float x = position.x;
  for (char character : text) {
    if (character == ' ') {
      x += 4.F * scale;
      continue;
    }
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

void drawInkText(sf::RenderWindow& window, std::string_view text, sf::Vector2f position,
                 float scale, sf::Color color = sf::Color(75, 66, 64)) {
  drawBitmapText(window, text, position + sf::Vector2f{2.F, 3.F}, scale, sf::Color(38, 28, 27, 90));
  drawBitmapText(window, text, position, scale, color);
}

bool drawTextureFill(sf::RenderWindow& window, isaac::resource::ResourceManager& resources,
                     const std::filesystem::path& path) {
  const auto texture = resources.texture(path);
  if (!texture) return false;
  sf::Sprite sprite(*texture);
  const auto size = texture->getSize();
  sprite.setScale({static_cast<float>(window.getSize().x) / static_cast<float>(size.x),
                   static_cast<float>(window.getSize().y) / static_cast<float>(size.y)});
  window.draw(sprite);
  return true;
}

bool drawTextureSprite(sf::RenderWindow& window, isaac::resource::ResourceManager& resources,
                       const std::filesystem::path& path, sf::Vector2f position,
                       float targetHeight, sf::Color tint = sf::Color::White,
                       float rotationDegrees = 0.F) {
  const auto texture = resources.texture(path);
  if (!texture) return false;
  sf::Sprite sprite(*texture);
  const auto size = texture->getSize();
  sprite.setOrigin({static_cast<float>(size.x) / 2.F, static_cast<float>(size.y) / 2.F});
  const float scale = targetHeight / static_cast<float>(size.y);
  sprite.setScale({scale, scale});
  sprite.setPosition(position);
  sprite.setColor(tint);
  sprite.setRotation(sf::degrees(rotationDegrees));
  window.draw(sprite);
  return true;
}

bool drawMaskedSprite(sf::RenderWindow& window, isaac::resource::ResourceManager& resources,
                      const std::filesystem::path& colorPath, const std::filesystem::path& maskPath,
                      sf::Vector2f position, float targetHeight, sf::Color tint = sf::Color::White,
                      float rotationDegrees = 0.F) {
  const auto texture = resources.maskedTexture(colorPath, maskPath);
  if (!texture) return false;
  sf::Sprite sprite(*texture);
  const auto size = texture->getSize();
  sprite.setOrigin({static_cast<float>(size.x) / 2.F, static_cast<float>(size.y) / 2.F});
  const float scale = targetHeight / static_cast<float>(size.y);
  sprite.setScale({scale, scale});
  sprite.setPosition(position);
  sprite.setColor(tint);
  sprite.setRotation(sf::degrees(rotationDegrees));
  window.draw(sprite);
  return true;
}

void drawShadow(sf::RenderWindow& window, sf::Vector2f position, float radius) {
  sf::CircleShape shadow(radius);
  shadow.setOrigin({radius, radius});
  shadow.setPosition(position + sf::Vector2f{0.F, radius * 0.55F});
  shadow.setScale({1.35F, 0.42F});
  shadow.setFillColor(sf::Color(12, 5, 7, 105));
  window.draw(shadow);
}

sf::Color characterTint(isaac::presentation::CharacterStyle style) {
  using Style = isaac::presentation::CharacterStyle;
  if (style == Style::Magdalene) return sf::Color(255, 178, 178);
  if (style == Style::Cain) return sf::Color(244, 212, 142);
  if (style == Style::Judas) return sf::Color(170, 160, 170);
  return sf::Color::White;
}

std::filesystem::path characterPortrait(isaac::presentation::CharacterStyle style, bool closed) {
  using Style = isaac::presentation::CharacterStyle;
  if (style == Style::Magdalene) return isaac::resource::AssetCatalog::magdalenePortrait(closed);
  if (style == Style::Cain) return isaac::resource::AssetCatalog::cainPortrait(closed);
  if (style == Style::Judas) return isaac::resource::AssetCatalog::judasPortrait(closed);
  return isaac::resource::AssetCatalog::isaac();
}

struct DoorPose {
  sf::Vector2f position;
  float rotationDegrees{};
};

DoorPose doorPose(isaac::common::Direction direction) {
  using isaac::common::Direction;
  if (direction == Direction::Right) return {{865.F, 300.F}, 90.F};
  if (direction == Direction::Down) return {{480.F, 535.F}, 180.F};
  if (direction == Direction::Left) return {{95.F, 300.F}, 270.F};
  return {{480.F, 95.F}, 0.F};
}

float woodDoorRotation(isaac::common::Direction direction) {
  using isaac::common::Direction;
  if (direction == Direction::Right) return 270.F;
  if (direction == Direction::Down) return 0.F;
  if (direction == Direction::Left) return 90.F;
  return 180.F;
}

std::filesystem::path treasureDoorTexture(isaac::common::Direction direction) {
  using isaac::common::Direction;
  if (direction == Direction::Right) return isaac::resource::AssetCatalog::treasureDoorRight();
  if (direction == Direction::Down) return isaac::resource::AssetCatalog::treasureDoorDown();
  if (direction == Direction::Left) return isaac::resource::AssetCatalog::treasureDoorLeft();
  return isaac::resource::AssetCatalog::treasureDoorUp();
}

float treasureDoorTargetHeight(isaac::common::Direction direction) {
  using isaac::common::Direction;
  return direction == Direction::Left || direction == Direction::Right ? 111.34F : 102.F;
}

std::string_view treasurePropStem(std::string_view itemId) {
  if (itemId == "sad_onion") return "prop5";
  if (itemId == "wiggle_worm") return "prop6";
  return {};
}

void drawDoorSeal(sf::RenderWindow& window, sf::Vector2f position) {
  for (const float rotation : {-28.F, 28.F}) {
    sf::RectangleShape bar({58.F, 7.F});
    bar.setOrigin({29.F, 3.5F});
    bar.setPosition(position);
    bar.setRotation(sf::degrees(rotation));
    bar.setFillColor(sf::Color(74, 45, 37));
    bar.setOutlineThickness(1.F);
    bar.setOutlineColor(sf::Color(28, 16, 17));
    window.draw(bar);
  }
}

}  // namespace

namespace isaac::view {

GameView::GameView(resource::ResourceManager& resources)
    : resources_(resources),
      window_(sf::VideoMode({960U, 640U}), "ISA - EasyIsaac final presentation") {
  window_.setVerticalSyncEnabled(true);
  window_.setKeyRepeatEnabled(false);
  window_.setPosition({100, 60});

  shootBuffer_ = resources_.soundBuffer(resource::AssetCatalog::easySound("shoot.wav"));
  hurtBuffer_ = resources_.soundBuffer(resource::AssetCatalog::easySound("hurt0.mp3"));
  pickupBuffer_ = resources_.soundBuffer(resource::AssetCatalog::easySound("getProp.mp3"));
  defeatBuffer_ = resources_.soundBuffer(resource::AssetCatalog::easySound("dead.mp3"));
  if (shootBuffer_) shootSound_.emplace(*shootBuffer_);
  if (hurtBuffer_) hurtSound_.emplace(*hurtBuffer_);
  if (pickupBuffer_) pickupSound_.emplace(*pickupBuffer_);
  if (defeatBuffer_) defeatSound_.emplace(*defeatBuffer_);
  if (shootSound_) shootSound_->setVolume(45.F);
  if (hurtSound_) hurtSound_->setVolume(55.F);
  if (pickupSound_) pickupSound_->setVolume(55.F);
  if (defeatSound_) defeatSound_->setVolume(60.F);
}

void GameView::setDisplay(const presentation::DisplayState& display) {
  if (display.screen != display_.screen) transitionClock_.restart();
  display_ = display;
}

void GameView::present(presentation::PresentationEvent event) {
  if (event == presentation::PresentationEvent::Shot && shootSound_) shootSound_->play();
  if (event == presentation::PresentationEvent::Pickup && pickupSound_) pickupSound_->play();
  if (event == presentation::PresentationEvent::Defeat && defeatSound_) defeatSound_->play();
  if (event == presentation::PresentationEvent::Hurt) {
    damageFlashClock_.restart();
    damageFlashActive_ = true;
    if (hurtSound_) hurtSound_->play();
  }
}

void GameView::pollEvents() {
  using Key = sf::Keyboard::Key;
  using Action = presentation::UserAction;
  const auto sendAction = [this](Action action) {
    if (actionHandler_) actionHandler_(action);
  };
  while (const auto event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) window_.close();
    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->code == Key::U) showHitboxes_ = !showHitboxes_;
      if (key->code == Key::Enter) sendAction(Action::Confirm);
      if (key->code == Key::Escape) sendAction(Action::Back);
      if (key->code == Key::E) sendAction(Action::UseBomb);
      if (key->code == Key::W) sendAction(Action::NavigateUp);
      if (key->code == Key::S) sendAction(Action::NavigateDown);
      if (key->code == Key::A) sendAction(Action::NavigateLeft);
      if (key->code == Key::D) sendAction(Action::NavigateRight);
      if (key->code == Key::Space) {
        const auto action = display_.screen == common::ScreenState::Playing ? Action::UseActive
                                                                            : Action::Confirm;
        sendAction(action);
      }
    }
  }
  presentation::RealtimeInput input;
  input.movement = {
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::D)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::A)),
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::S)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::W))};
  input.shooting = {
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::Right)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::Left)),
      static_cast<float>(sf::Keyboard::isKeyPressed(Key::Down)) - static_cast<float>(sf::Keyboard::isKeyPressed(Key::Up))};
  if (inputHandler_) inputHandler_(input);
}

void GameView::render() {
  const auto& display = display_;
  const float time = animationClock_.getElapsedTime().asSeconds();

  window_.clear(sf::Color(20, 12, 14));

  const auto drawPaper = [&] {
    if (!drawTextureFill(window_, resources_, resource::AssetCatalog::easyPanel("menu.jpg"))) {
      sf::RectangleShape paper({960.F, 640.F});
      paper.setFillColor(sf::Color(221, 214, 196));
      window_.draw(paper);
    }
  };

  if (display.screen == common::ScreenState::Start) {
    const auto frame = static_cast<int>(time * 2.F) % 2 == 0 ? "begin0.jpg" : "begin1.jpg";
    drawTextureFill(window_, resources_, resource::AssetCatalog::easyPanel(frame));
    drawInkText(window_, "PRESS ENTER", {345.F, 560.F}, 3.F, sf::Color(98, 74, 69));
  } else if (display.screen == common::ScreenState::MainMenu) {
    drawPaper();
    drawInkText(window_, "ISA CLASSIC", {330.F, 92.F}, 5.F, sf::Color(145, 20, 23));
    constexpr std::array<std::string_view, 3> options{"RUN", "RANKINGS", "BACK"};
    for (std::size_t index = 0; index < options.size(); ++index) {
      const bool selected = static_cast<int>(index) == display.menuIndex;
      drawInkText(window_, options[index], {330.F + (selected ? 8.F : 0.F), 235.F + 95.F * static_cast<float>(index)},
                  4.F, selected ? sf::Color(182, 20, 24) : sf::Color(83, 72, 70));
    }
  } else if (display.screen == common::ScreenState::Rankings) {
    drawPaper();
    drawInkText(window_, "RANKINGS", {345.F, 105.F}, 5.F, sf::Color(125, 30, 31));
    drawInkText(window_, "COURSE RUN", {275.F, 220.F}, 4.F);
    drawInkText(window_, "2 FLOORS", {275.F, 300.F}, 3.5F);
    drawInkText(window_, "3 BOSSES", {540.F, 300.F}, 3.5F);
    drawInkText(window_, "ACCEPTANCE PASS", {275.F, 390.F}, 3.5F);
    drawInkText(window_, "ENTER OR ESC TO RETURN", {255.F, 540.F}, 2.F);
  } else if (display.screen == common::ScreenState::CharacterSelect) {
    drawPaper();
    drawInkText(window_, "WHO ARE YOU", {330.F, 82.F}, 4.5F, sf::Color(135, 30, 33));
    const bool blink = std::fmod(time, 3.2F) > 2.95F;
    drawTextureSprite(window_, resources_, characterPortrait(display.selectionStyle, blink),
                      {480.F, 285.F + std::sin(time * 4.F) * 5.F}, 120.F);
    drawInkText(window_, "A  <  " + display.selectionName + "  >  D", {285.F, 385.F}, 3.F);
    drawInkText(window_, display.selectionStats, {255.F, 450.F}, 2.F);
    drawInkText(window_, "ENTER TO BEGIN", {345.F, 535.F}, 2.5F, sf::Color(155, 28, 30));
  } else if (display.screen == common::ScreenState::Defeat || display.screen == common::ScreenState::Victory) {
    drawPaper();
    drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyPanel("testament_back.jpg"),
                     resource::AssetCatalog::easyPanel("testament_front.jpg"),
                     {480.F, 320.F}, 500.F);
    drawInkText(window_, display.screen == common::ScreenState::Defeat ? "DEAR DIARY" : "RUN COMPLETE",
                {335.F, 145.F}, 4.F, sf::Color(135, 24, 27));
    std::ostringstream result;
    result << "FLOOR " << display.hud.floor << "  TIME " << static_cast<int>(display.hud.elapsedSeconds) << "S";
    drawInkText(window_, result.str(), {330.F, 390.F}, 2.5F);
    drawInkText(window_, "ENTER FOR MENU", {345.F, 480.F}, 2.5F);
  } else {
    const auto background = display.hud.floor % 2 == 0 ? "BK1.jpg" : "BK0.jpg";
    if (!drawTextureFill(window_, resources_, resource::AssetCatalog::easyPanel(background))) {
      sf::RectangleShape room({880.F, 500.F});
      room.setPosition({40.F, 70.F});
      room.setFillColor(sf::Color(105, 65, 60));
      window_.draw(room);
    }

    for (const auto& door : display.doors) {
      const auto pose = doorPose(door.direction);
      const auto tint = door.sealed ? sf::Color(105, 82, 80) : sf::Color::White;
      auto texture = resource::AssetCatalog::woodDoor();
      auto targetHeight = 158.6F;
      auto rotation = woodDoorRotation(door.direction);
      if (door.targetType == common::RoomType::Treasure) {
        texture = treasureDoorTexture(door.direction);
        targetHeight = treasureDoorTargetHeight(door.direction);
        rotation = 0.F;
      } else if (door.targetType == common::RoomType::Boss) {
        texture = resource::AssetCatalog::bossDoor();
        targetHeight = 67.5F;
      }
      const bool drawn = drawTextureSprite(window_, resources_, texture, pose.position,
                                           targetHeight, tint, rotation);
      if (!drawn) {
        sf::RectangleShape fallback({64.F, 34.F});
        fallback.setOrigin({32.F, 17.F});
        fallback.setPosition(pose.position);
        fallback.setRotation(sf::degrees(pose.rotationDegrees));
        fallback.setFillColor(door.sealed ? sf::Color(65, 42, 42) : sf::Color(135, 74, 61));
        fallback.setOutlineThickness(3.F);
        fallback.setOutlineColor(sf::Color(35, 20, 22));
        window_.draw(fallback);
      }
      if (door.sealed) drawDoorSeal(window_, pose.position);
    }

    if (display.trapdoorVisible) {
      if (!drawTextureSprite(window_, resources_, resource::AssetCatalog::trapdoor(),
                             {480.F, 385.F}, 54.F)) {
        sf::RectangleShape fallback({52.F, 42.F});
        fallback.setOrigin({26.F, 21.F});
        fallback.setPosition({480.F, 385.F});
        fallback.setFillColor(sf::Color(32, 22, 22));
        fallback.setOutlineThickness(4.F);
        fallback.setOutlineColor(sf::Color(92, 62, 52));
        window_.draw(fallback);
      }
    }

    const auto shooting = display.shooting.lengthSquared() > 0.1F;
    const auto movement = display.movement;
    for (const auto& entity : display.entities) {
      const sf::Vector2f position{entity.position.x, entity.position.y};
      drawShadow(window_, position, std::max(6.F, entity.radius));
      bool drawn = false;

      if (entity.kind == common::EntityKind::Rock) {
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyObstacle("stone0_back.jpg"),
                     resource::AssetCatalog::easyObstacle("stone0_front.jpg"),
                     position, 58.F, sf::Color(255, 255, 255, 210));
      } else if (entity.kind == common::EntityKind::Trap) {
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyObstacle("spine0_back.jpg"),
                     resource::AssetCatalog::easyObstacle("spine0_front.jpg"),
                     position, 48.F, sf::Color(255, 255, 255, 190));
      } else if (entity.kind == common::EntityKind::TreasureItem) {
        const auto stem = treasurePropStem(entity.itemId);
        if (!stem.empty()) {
          drawn = drawMaskedSprite(window_, resources_,
                       resource::AssetCatalog::easyProp(std::string(stem) + "_back.jpg"),
                       resource::AssetCatalog::easyProp(std::string(stem) + "_front.jpg"),
                       position + sf::Vector2f{0.F, std::sin(time * 6.F) * 4.F}, 52.F);
        }
      } else if (entity.kind == common::EntityKind::Player) {
        std::string body = "isaac_walk0_back.jpg";
        std::string bodyMask = "isaac_walk0_front.jpg";
        if (movement.x > 0.1F) { body = "isaac_walk0_side_back.jpg"; bodyMask = "isaac_walk0_side_front.jpg"; }
        if (movement.x < -0.1F) { body = "isaac_walk0_left_back.jpg"; bodyMask = "isaac_walk0_left_front.jpg"; }
        const float bob = std::sin(time * 9.F) * (movement.lengthSquared() > 0.1F ? 3.F : 1.F);
        const auto tint = characterTint(entity.characterStyle);
        drawMaskedSprite(window_, resources_, resource::AssetCatalog::easyCharacter(body),
                         resource::AssetCatalog::easyCharacter(bodyMask), position + sf::Vector2f{0.F, 12.F + bob}, 42.F, tint);
        if (entity.characterStyle == presentation::CharacterStyle::Isaac) {
          drawn = drawMaskedSprite(window_, resources_,
                       resource::AssetCatalog::easyCharacter(shooting ? "isaac0_shoot_back.jpg" : "isaac0_back.jpg"),
                       resource::AssetCatalog::easyCharacter(shooting ? "isaac0_shoot_front.jpg" : "isaac0_front.jpg"),
                       position + sf::Vector2f{0.F, -12.F + bob}, 52.F, tint);
        } else {
          const bool blink = !shooting && std::fmod(time, 3.2F) > 2.95F;
          drawn = drawTextureSprite(window_, resources_,
                                    characterPortrait(entity.characterStyle, blink),
                                    position + sf::Vector2f{0.F, -12.F + bob}, 52.F);
        }
      } else if (entity.kind == common::EntityKind::Enemy) {
        const float pulse = 72.F + std::sin(time * 12.F + entity.position.x * 0.03F) * 6.F;
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyMonster("monster_redfly0_back.jpg"),
                     resource::AssetCatalog::easyMonster("monster_redfly0_front.jpg"),
                     position, pulse, sf::Color(255, 150, 150));
      } else if (entity.kind == common::EntityKind::Boss) {
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyMonster("people_head_back.jpg"),
                     resource::AssetCatalog::easyMonster("people_head_front.jpg"),
                     position + sf::Vector2f{0.F, std::sin(time * 5.F) * 5.F}, entity.radius * 2.5F,
                     sf::Color(255, 125, 125));
      } else if (entity.kind == common::EntityKind::PlayerProjectile ||
                 entity.kind == common::EntityKind::EnemyProjectile) {
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyBullet("bullet0_back.jpg"),
                     resource::AssetCatalog::easyBullet("bullet0_front.jpg"),
                     position, 60.F,
                     entity.kind == common::EntityKind::PlayerProjectile ? sf::Color::White : sf::Color(255, 120, 120),
                     std::fmod(time * 180.F + entity.position.x, 360.F));
      } else if (entity.kind == common::EntityKind::Pickup) {
        drawn = drawMaskedSprite(window_, resources_,
                     resource::AssetCatalog::easyProp("prop0_back.jpg"),
                     resource::AssetCatalog::easyProp("prop0_front.jpg"),
                     position + sf::Vector2f{0.F, std::sin(time * 6.F) * 4.F}, 36.F);
      }

      if (!drawn) {
        sf::CircleShape fallback(entity.radius);
        fallback.setOrigin({entity.radius, entity.radius});
        fallback.setPosition(position);
        auto fallbackColor = sf::Color(150, 45, 55);
        if (entity.kind == common::EntityKind::Player) fallbackColor = sf::Color(220, 187, 166);
        if (entity.kind == common::EntityKind::Rock) fallbackColor = sf::Color(92, 70, 65);
        if (entity.kind == common::EntityKind::Trap) fallbackColor = sf::Color(150, 150, 145);
        if (entity.kind == common::EntityKind::TreasureItem) fallbackColor = sf::Color(220, 190, 95);
        fallback.setFillColor(fallbackColor);
        window_.draw(fallback);
      }
      if (showHitboxes_) {
        sf::CircleShape hitbox(entity.radius);
        hitbox.setOrigin({entity.radius, entity.radius});
        hitbox.setPosition(position);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineThickness(1.F);
        hitbox.setOutlineColor(sf::Color::Green);
        window_.draw(hitbox);
      }
    }

    const auto drawHeart = [&](std::string_view stem, int slot) {
      drawMaskedSprite(window_, resources_,
                       resource::AssetCatalog::easyPanel(std::string(stem) + "_back.jpg"),
                       resource::AssetCatalog::easyPanel(std::string(stem) + "_front.jpg"),
                       {125.F + 34.F * static_cast<float>(slot), 82.F}, 28.F);
    };
    for (int i = 0; i < display.hud.redHearts; ++i) {
      drawHeart("health0", i);
    }
    int occupiedRedSlots = display.hud.redHearts;
    if (display.hud.redHalfHeart) drawHeart("health1", occupiedRedSlots++);
    for (int slot = occupiedRedSlots; slot < display.hud.heartContainers; ++slot) {
      drawHeart("health2", slot);
    }
    for (int i = 0; i < display.hud.shields; ++i) {
      drawHeart("health3", display.hud.heartContainers + i);
    }
    if (display.hud.shieldHalfHeart) {
      drawHeart("health4", display.hud.heartContainers + display.hud.shields);
    }

    const auto stat = [&](std::string_view name, float value, float y) {
      std::ostringstream number;
      number << std::fixed << std::setprecision(value < 20.F ? 1 : 0) << value;
      drawInkText(window_, name, {8.F, y}, 1.8F, sf::Color(245, 238, 224));
      drawInkText(window_, number.str(), {18.F, y + 23.F}, 1.5F, sf::Color::White);
    };
    stat("SPEED", display.hud.moveSpeed, 130.F);
    stat("TEARS", display.hud.shotsPerSecond, 205.F);
    stat("DAMAGE", display.hud.damage, 280.F);
    stat("RANGE", 450.F, 355.F);
    stat("SHOT SPEED", display.hud.projectileSpeed, 430.F);

    std::ostringstream top;
    top << "TIME " << static_cast<int>(display.hud.elapsedSeconds) << "S  " << display.hud.roomState;
    drawInkText(window_, top.str(), {350.F, 24.F}, 2.F, sf::Color::White);
    std::ostringstream resources;
    resources << "C " << display.hud.coins << "  B " << display.hud.bombs << "  K " << display.hud.keys;
    drawInkText(window_, resources.str(), {715.F, 20.F}, 1.8F, sf::Color::White);
    drawInkText(window_, "ACTIVE " + display.hud.activeItem, {705.F, 50.F}, 1.3F, sf::Color(230, 217, 197));
    drawInkText(window_, "FLOOR " + std::to_string(display.hud.floor), {410.F, 598.F}, 2.5F, sf::Color::White);

    for (const auto& roomState : display.minimap) {
      sf::RectangleShape cell({14.F, 10.F});
      cell.setPosition({820.F + 18.F * static_cast<float>(roomState.x),
                        92.F + 14.F * static_cast<float>(roomState.y)});
      cell.setFillColor(roomState.current ? sf::Color(235, 210, 120) : sf::Color(170, 154, 145));
      cell.setOutlineThickness(1.F);
      cell.setOutlineColor(sf::Color(48, 35, 34));
      window_.draw(cell);
    }

    if (display.screen == common::ScreenState::Paused) {
      sf::RectangleShape dim({960.F, 640.F});
      dim.setFillColor(sf::Color(10, 5, 8, 145));
      window_.draw(dim);
      const auto pauseFrame = display.pauseMenuIndex;
      drawMaskedSprite(window_, resources_,
                       resource::AssetCatalog::easyPanel(pauseFrame == 0 ? "pause0_back.jpg" : "pause1_back.jpg"),
                       resource::AssetCatalog::easyPanel(pauseFrame == 0 ? "pause0_front.jpg" : "pause1_front.jpg"),
                       {480.F, 320.F}, 420.F);
      drawInkText(window_, "ESC", {445.F, 452.F}, 1.6F, sf::Color(95, 80, 75));
    }
  }

  const float fadeSeconds = transitionClock_.getElapsedTime().asSeconds();
  if (fadeSeconds < 0.3F) {
    const auto alpha = static_cast<std::uint8_t>(180.F * (1.F - fadeSeconds / 0.3F));
    sf::RectangleShape fade({960.F, 640.F});
    fade.setFillColor(sf::Color(20, 8, 10, alpha));
    window_.draw(fade);
  }
  const float damageSeconds = damageFlashClock_.getElapsedTime().asSeconds();
  if (damageFlashActive_ && damageSeconds < 0.18F) {
    const auto alpha = static_cast<std::uint8_t>(105.F * (1.F - damageSeconds / 0.18F));
    sf::RectangleShape flash({960.F, 640.F});
    flash.setFillColor(sf::Color(190, 20, 30, alpha));
    window_.draw(flash);
  } else if (damageFlashActive_) {
    damageFlashActive_ = false;
  }
  window_.display();
}

}  // namespace isaac::view
