#pragma once

#include "model/Player.h"

#include <array>
#include <string_view>

namespace isaac::model {

enum class ItemCategory { Active, Passive, Trinket };
enum class ItemEffect { Heal, HealthUp, DamageUp, TearsUp, SineTears, LuckUp };

struct ItemDefinition {
  std::string_view id;
  std::string_view displayName;
  ItemCategory category;
  ItemEffect effect;
  float amount;
};

class ItemCatalog {
 public:
  static const std::array<ItemDefinition, 7>& all();
  static const ItemDefinition& byId(std::string_view id);
};

class ItemSystem {
 public:
  void apply(Player& player, const ItemDefinition& item) const;
  bool useActive(Player& player) const;
  bool takeTreasureItem(Player& player, std::string_view itemId);
  bool buyShopItem(Player& player, int price = 5);
  bool takeSecretTrinket(Player& player);
  [[nodiscard]] bool treasureTaken() const { return treasureTaken_; }
  [[nodiscard]] bool shopSold() const { return shopSold_; }
  [[nodiscard]] bool secretTaken() const { return secretTaken_; }

 private:
  bool treasureTaken_{};
  bool shopSold_{};
  bool secretTaken_{};
};

}  // namespace isaac::model
