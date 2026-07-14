#include "resource/ResourceManager.h"

#include <SFML/Graphics/Image.hpp>

#include <algorithm>

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

std::shared_ptr<sf::Texture> ResourceManager::maskedTexture(
    const std::filesystem::path& colorPath, const std::filesystem::path& maskPath) {
  const auto colorKey = colorPath.lexically_normal().generic_string();
  const auto maskKey = maskPath.lexically_normal().generic_string();
  const auto key = "masked:" + colorKey + '|' + maskKey;
  if (const auto found = textures_.find(key); found != textures_.end()) return found->second;

  sf::Image color;
  sf::Image mask;
  if (!color.loadFromFile(colorPath) || !mask.loadFromFile(maskPath)) {
    lastDiagnostic_ = "Optional EasyIsaac mask pair missing: " + colorKey + " / " + maskKey;
    return {};
  }

  const auto size = color.getSize();
  const auto maskSize = mask.getSize();
  const int maskOffsetX = (static_cast<int>(maskSize.x) - static_cast<int>(size.x)) / 2;
  const int maskOffsetY = (static_cast<int>(maskSize.y) - static_cast<int>(size.y)) / 2;
  for (unsigned int y = 0; y < size.y; ++y) {
    for (unsigned int x = 0; x < size.x; ++x) {
      auto pixel = color.getPixel({x, y});
      const int maskX = static_cast<int>(x) + maskOffsetX;
      const int maskY = static_cast<int>(y) + maskOffsetY;
      const auto maskPixel = maskX >= 0 && maskY >= 0 && maskX < static_cast<int>(maskSize.x) &&
                                     maskY < static_cast<int>(maskSize.y)
                                 ? mask.getPixel({static_cast<unsigned int>(maskX),
                                                  static_cast<unsigned int>(maskY)})
                                 : sf::Color::White;
      const auto brightness = (static_cast<unsigned int>(maskPixel.r) + maskPixel.g + maskPixel.b) / 3U;
      auto alpha = 255U - brightness;
      if (alpha < 28U) alpha = 0U;
      if (alpha > 224U) alpha = 255U;
      pixel.a = static_cast<std::uint8_t>(alpha);
      if (pixel.a == 0U) pixel = sf::Color::Transparent;
      color.setPixel({x, y}, pixel);
    }
  }

  auto loaded = std::make_shared<sf::Texture>();
  if (!loaded->loadFromImage(color)) {
    lastDiagnostic_ = "Unable to build EasyIsaac alpha texture: " + colorKey;
    return {};
  }
  loaded->setSmooth(false);
  textures_.emplace(key, loaded);
  return loaded;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::soundBuffer(const std::filesystem::path& path) {
  const auto key = path.lexically_normal().generic_string();
  if (const auto found = soundBuffers_.find(key); found != soundBuffers_.end()) return found->second;
  auto loaded = std::make_shared<sf::SoundBuffer>();
  if (!loaded->loadFromFile(path)) {
    lastDiagnostic_ = "Optional sound missing: " + key + "; continuing without audio.";
    return {};
  }
  soundBuffers_.emplace(key, loaded);
  return loaded;
}

}  // namespace isaac::resource
