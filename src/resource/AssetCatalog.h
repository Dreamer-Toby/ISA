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
  static std::filesystem::path door() { return "assets/textures/rooms/red-room-door.png"; }
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
