#pragma once

#include <filesystem>

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
};

}  // namespace isaac::resource
