#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace isaac::resource {

class ResourceManager {
 public:
  [[nodiscard]] std::shared_ptr<sf::Texture> texture(const std::filesystem::path& path);
  [[nodiscard]] const std::string& lastDiagnostic() const { return lastDiagnostic_; }
  [[nodiscard]] std::size_t textureCount() const { return textures_.size(); }

 private:
  std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures_;
  std::string lastDiagnostic_;
};

}  // namespace isaac::resource

