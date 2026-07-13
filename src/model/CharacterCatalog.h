#pragma once

#include <array>
#include <string_view>

namespace isaac::model {

struct CharacterDefinition {
  std::string_view id;
  std::string_view displayName;
  int heartContainers;
  int redHearts;
  int startingCoins;
  float moveSpeed;
  float damage;
  float shotsPerSecond;
  float luck;
};

class CharacterCatalog {
 public:
  static const std::array<CharacterDefinition, 4>& all();
  static const CharacterDefinition& at(std::size_t index);
};

}  // namespace isaac::model

