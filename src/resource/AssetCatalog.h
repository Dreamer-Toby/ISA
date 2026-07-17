#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace isaac::resource {

class AssetCatalog {
 public:
  static std::filesystem::path isaac() { return "assets/textures/characters/isaac-icon.png"; }
  static std::filesystem::path magdalene() { return "assets/textures/characters/magdalene-icon.png"; }
  static std::filesystem::path cain() { return "assets/textures/characters/cain-icon.png"; }
  static std::filesystem::path judas() { return "assets/textures/characters/judas-icon.png"; }
  static std::filesystem::path fly() { return "assets/textures/enemies/fly.png"; }
  static std::filesystem::path boss() { return "assets/textures/bosses/boss-icon.png"; }
  static std::filesystem::path tear() { return "assets/textures/items/blood-tear.png"; }
  static std::filesystem::path woodDoor() {
    return "assets/textures/rooms/wood-door.png";
  }
  static std::filesystem::path treasureDoorUp() {
    return "assets/textures/rooms/treasure-door-up.png";
  }
  static std::filesystem::path treasureDoorRight() {
    return "assets/textures/rooms/treasure-door-right.png";
  }
  static std::filesystem::path treasureDoorDown() {
    return "assets/textures/rooms/treasure-door-down.png";
  }
  static std::filesystem::path treasureDoorLeft() {
    return "assets/textures/rooms/treasure-door-left.png";
  }
  static std::filesystem::path bossDoor() { return "assets/textures/rooms/boss-door.png"; }
  static std::filesystem::path trapdoor() { return "assets/textures/rooms/trapdoor.png"; }
  static std::filesystem::path cainPortrait(bool closed) {
    return closed ? "assets/textures/characters/cain-portrait-closed.png"
                  : "assets/textures/characters/cain-portrait.png";
  }
  static std::filesystem::path judasPortrait(bool closed) {
    return closed ? "assets/textures/characters/judas-portrait-closed.png"
                  : "assets/textures/characters/judas-portrait.png";
  }
  static std::filesystem::path magdalenePortrait(bool closed) {
    return closed ? "assets/textures/characters/magdalene-portrait-closed.png"
                  : "assets/textures/characters/magdalene-portrait.png";
  }
  static std::filesystem::path redHeart() { return "assets/textures/ui/red-heart.png"; }
  static std::filesystem::path shieldHeart() { return "assets/textures/ui/shield-heart.png"; }

  static std::filesystem::path easyPanel(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/panel") / std::string(name);
  }
  static std::filesystem::path easyCharacter(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/character") / std::string(name);
  }
  static std::filesystem::path easyMonster(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/monster") / std::string(name);
  }
  static std::filesystem::path easyBullet(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/bullet") / std::string(name);
  }
  static std::filesystem::path easyObstacle(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/obstacle") / std::string(name);
  }
  static std::filesystem::path easyProp(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/prop") / std::string(name);
  }
  static std::filesystem::path easyDrop(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/drops") / std::string(name);
  }
  static std::filesystem::path easySound(std::string_view name) {
    return std::filesystem::path("assets/easyisaac/sounds") / std::string(name);
  }
};

}  // namespace isaac::resource
