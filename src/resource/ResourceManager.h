#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace isaac::resource {

class ResourceManager {
 public:
  [[nodiscard]] std::shared_ptr<sf::Texture> texture(const std::filesystem::path& path);
  [[nodiscard]] std::shared_ptr<sf::Texture> maskedTexture(
      const std::filesystem::path& colorPath, const std::filesystem::path& maskPath);
  [[nodiscard]] std::shared_ptr<sf::SoundBuffer> soundBuffer(const std::filesystem::path& path);
  [[nodiscard]] const std::string& lastDiagnostic() const { return lastDiagnostic_; }
  [[nodiscard]] std::size_t textureCount() const { return textures_.size(); }
  [[nodiscard]] std::size_t soundBufferCount() const { return soundBuffers_.size(); }

 private:
  std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures_;
  std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers_;
  std::string lastDiagnostic_;
};

}  // namespace isaac::resource
