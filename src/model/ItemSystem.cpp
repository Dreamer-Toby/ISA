#include "model/ItemSystem.h"

#include <stdexcept>

namespace isaac::model {

const std::array<ItemDefinition, 5>& ItemCatalog::all() {
  static constexpr std::array<ItemDefinition, 5> items{{
      {"yum_heart", "Yum Heart", ItemCategory::Active, ItemEffect::Heal, 1.F},
      {"small_rock", "Small Rock", ItemCategory::Passive, ItemEffect::DamageUp, 1.F},
      {"sad_onion", "Sad Onion", ItemCategory::Passive, ItemEffect::TearsUp, 0.85F},
      {"lucky_toe", "Lucky Toe", ItemCategory::Trinket, ItemEffect::LuckUp, 1.F},
      {"book_of_belial", "Book of Belial", ItemCategory::Active, ItemEffect::DamageUp, 2.F},
  }};
  return items;
}

const ItemDefinition& ItemCatalog::byId(std::string_view id) {
  for (const auto& item : all()) if (item.id == id) return item;
  throw std::out_of_range("unknown item id");
}

void ItemSystem::apply(Player& player, const ItemDefinition& item) const {
  if (item.category == ItemCategory::Active) player.inventory().setActiveItem(std::string(item.displayName));
  if (item.category == ItemCategory::Passive) {
    player.inventory().addPassiveItem(std::string(item.displayName));
    if (item.effect == ItemEffect::DamageUp) player.shooting().addDamage(item.amount);
    if (item.effect == ItemEffect::TearsUp) player.shooting().multiplyInterval(item.amount);
  }
  if (item.category == ItemCategory::Trinket) {
    player.inventory().setTrinket(std::string(item.displayName));
    if (item.effect == ItemEffect::LuckUp) player.addLuck(item.amount);
  }
}

bool ItemSystem::useActive(Player& player) const {
  if (player.inventory().activeItem() == "Yum Heart") {
    const int before = player.health().red();
    player.health().healRed(1);
    return player.health().red() > before;
  }
  if (player.inventory().activeItem() == "Book of Belial") {
    player.shooting().addDamage(2.F);
    return true;
  }
  return false;
}

bool ItemSystem::openChest(Player& player) {
  if (chestOpened_ || !player.inventory().useKey()) return false;
  chestOpened_ = true;
  apply(player, ItemCatalog::byId("sad_onion"));
  return true;
}

bool ItemSystem::buyShopItem(Player& player, int price) {
  if (shopSold_ || !player.inventory().spendCoins(price)) return false;
  shopSold_ = true;
  apply(player, ItemCatalog::byId("small_rock"));
  return true;
}

bool ItemSystem::takeSecretTrinket(Player& player) {
  if (secretTaken_) return false;
  secretTaken_ = true;
  apply(player, ItemCatalog::byId("lucky_toe"));
  return true;
}

}  // namespace isaac::model
