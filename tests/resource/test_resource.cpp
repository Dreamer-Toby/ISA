#include "resource/ResourceManager.h"

#include <SFML/Graphics/Image.hpp>

#include <array>
#include <cstdlib>
#include <filesystem>

namespace {

bool isVerticalMirror(const sf::Image& source, const sf::Image& mirrored) {
  const auto size = source.getSize();
  if (mirrored.getSize() != size) return false;
  for (unsigned int y = 0; y < size.y; ++y) {
    for (unsigned int x = 0; x < size.x; ++x) {
      if (source.getPixel({x, size.y - 1U - y}) != mirrored.getPixel({x, y})) return false;
    }
  }
  return true;
}

bool isHorizontalMirror(const sf::Image& source, const sf::Image& mirrored) {
  const auto size = source.getSize();
  if (mirrored.getSize() != size) return false;
  for (unsigned int y = 0; y < size.y; ++y) {
    for (unsigned int x = 0; x < size.x; ++x) {
      if (source.getPixel({size.x - 1U - x, y}) != mirrored.getPixel({x, y})) return false;
    }
  }
  return true;
}

}  // namespace

int main() {
  isaac::resource::ResourceManager resources;
  const auto path = std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/textures/ui/red-heart.png";
  const auto first = resources.texture(path);
  const auto second = resources.texture(path);
  if (!first || first != second || resources.textureCount() != 1) return EXIT_FAILURE;

  const auto easyRoot = std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/easyisaac";
  const auto maskedFirst = resources.maskedTexture(
      easyRoot / "character/isaac0_back.jpg", easyRoot / "character/isaac0_front.jpg");
  const auto maskedSecond = resources.maskedTexture(
      easyRoot / "character/isaac0_back.jpg", easyRoot / "character/isaac0_front.jpg");
  if (!maskedFirst || maskedFirst != maskedSecond || resources.textureCount() != 2) return EXIT_FAILURE;
  const auto mismatchedSizePair = resources.maskedTexture(
      easyRoot / "panel/health0_back.jpg", easyRoot / "panel/health0_front.jpg");
  if (!mismatchedSizePair || resources.textureCount() != 3) return EXIT_FAILURE;

  const auto assetRoot = std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/textures";
  constexpr std::array<const char*, 14> requiredRoomAndPortraitAssets{
      "rooms/normal-door-up.png",
      "rooms/normal-door-right.png",
      "rooms/normal-door-down.png",
      "rooms/normal-door-left.png",
      "rooms/boss-door.png",
      "rooms/treasure-door.png",
      "rooms/locked-treasure-door.png",
      "rooms/trapdoor.png",
      "characters/cain-portrait.png",
      "characters/cain-portrait-closed.png",
      "characters/judas-portrait.png",
      "characters/judas-portrait-closed.png",
      "characters/magdalene-portrait.png",
      "characters/magdalene-portrait-closed.png",
  };
  for (const auto* asset : requiredRoomAndPortraitAssets) {
    if (!resources.texture(assetRoot / asset)) return EXIT_FAILURE;
  }
  if (resources.textureCount() != 17) return EXIT_FAILURE;

  sf::Image downDoor;
  sf::Image upDoor;
  sf::Image leftDoor;
  sf::Image rightDoor;
  if (!downDoor.loadFromFile(assetRoot / "rooms/normal-door-down.png") ||
      !upDoor.loadFromFile(assetRoot / "rooms/normal-door-up.png") ||
      !leftDoor.loadFromFile(assetRoot / "rooms/normal-door-left.png") ||
      !rightDoor.loadFromFile(assetRoot / "rooms/normal-door-right.png")) {
    return EXIT_FAILURE;
  }
  if (!isVerticalMirror(downDoor, upDoor) || !isHorizontalMirror(leftDoor, rightDoor)) {
    return EXIT_FAILURE;
  }

  const auto soundFirst = resources.soundBuffer(easyRoot / "sounds/shoot.wav");
  const auto soundSecond = resources.soundBuffer(easyRoot / "sounds/shoot.wav");
  if (!soundFirst || soundFirst != soundSecond || resources.soundBufferCount() != 1) return EXIT_FAILURE;

  if (resources.texture(std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/missing.png")) return EXIT_FAILURE;
  if (resources.lastDiagnostic().find("fallback geometry") == std::string::npos) return EXIT_FAILURE;
  if (resources.soundBuffer(easyRoot / "sounds/missing.wav")) return EXIT_FAILURE;
  return resources.lastDiagnostic().find("without audio") != std::string::npos ? EXIT_SUCCESS : EXIT_FAILURE;
}
