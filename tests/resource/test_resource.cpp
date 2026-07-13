#include "resource/ResourceManager.h"

#include <cstdlib>
#include <filesystem>

int main() {
  isaac::resource::ResourceManager resources;
  const auto path = std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/textures/ui/red-heart.png";
  const auto first = resources.texture(path);
  const auto second = resources.texture(path);
  if (!first || first != second || resources.textureCount() != 1) return EXIT_FAILURE;
  if (resources.texture(std::filesystem::path(ISAAC_SOURCE_DIR) / "assets/missing.png")) return EXIT_FAILURE;
  return resources.lastDiagnostic().find("fallback geometry") != std::string::npos ? EXIT_SUCCESS : EXIT_FAILURE;
}
