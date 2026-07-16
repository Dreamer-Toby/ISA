#include "resource/ResourceManager.h"

#include <array>
#include <cstdlib>
#include <filesystem>

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
  constexpr std::array<const char*, 10> localMaterialAssets{
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
  for (const auto* asset : localMaterialAssets) {
    if (!resources.texture(assetRoot / asset)) return EXIT_FAILURE;
  }
  if (resources.textureCount() != 13) return EXIT_FAILURE;

  const auto soundFirst = resources.soundBuffer(easyRoot / "sounds/shoot.wav");
  const auto soundSecond = resources.soundBuffer(easyRoot / "sounds/shoot.wav");
  if (!soundFirst || soundFirst != soundSecond || resources.soundBufferCount() != 1) return EXIT_FAILURE;

  if (resources.texture(std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/missing.png")) return EXIT_FAILURE;
  if (resources.lastDiagnostic().find("fallback geometry") == std::string::npos) return EXIT_FAILURE;
  if (resources.soundBuffer(easyRoot / "sounds/missing.wav")) return EXIT_FAILURE;
  return resources.lastDiagnostic().find("without audio") != std::string::npos ? EXIT_SUCCESS : EXIT_FAILURE;
}
