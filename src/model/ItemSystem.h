#pragma once

#include "model/Player.h"

#include <array>
#include <string_view>

namespace isaac::model {

enum class ItemCategory { Active, Passive, Trinket };
enum class ItemEffect { Heal, DamageUp, TearsUp, LuckUp };

struct ItemDefinition {
  std::string_view id;
  std::string_view displayName;
  ItemCategory category;
  ItemEffect effect;
  float amount;
};

class ItemCatalog {
 public:
  static const std::array<ItemDefinition, 5>& all();
  static const ItemDefinition& byId(std::string_view id);
};

class ItemSystem {
 public:
  void apply(Player& player, const ItemDefinition& item) const;
  bool useActive(Player& player) const;
  bool openChest(Player& player);
  bool buyShopItem(Player& player, int price = 5);
  bool takeSecretTrinket(Player& player);
  [[nodiscard]] bool chestOpened() const { return chestOpened_; }
  [[nodiscard]] bool shopSold() const { return shopSold_; }

 private:
  bool chestOpened_{};
  bool shopSold_{};
  bool secretTaken_{};
};

}  // namespace isaac::model
