#include "resource/ResourceManager.h"

namespace isaac::resource {

std::shared_ptr<sf::Texture> ResourceManager::texture(const std::filesystem::path& path) {
  const auto key = path.lexically_normal().generic_string();
  if (const auto found = textures_.find(key); found != textures_.end()) {
    return found->second;
  }
  auto loaded = std::make_shared<sf::Texture>();
  if (!loaded->loadFromFile(path)) {
    lastDiagnostic_ = "Optional texture missing: " + key + "; using View fallback geometry.";
    return {};
  }
  loaded->setSmooth(false);
  textures_.emplace(key, loaded);
  return loaded;
}

}  // namespace isaac::resource

