#include "model/CharacterCatalog.h"
#include "model/Components.h"
#include "model/GameSession.h"
#include "model/Level.h"
#include "model/EnemySystem.h"
#include "model/ItemSystem.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
int failures{};
void check(bool condition, const char* message) {
  if (!condition) { std::cerr << "FAIL: " << message << '\n'; ++failures; }
}

isaac::model::SessionSnapshot simulateAtRenderRate(int framesPerSecond) {
  isaac::model::GameSession session;
  constexpr float fixed = 1.F / 60.F;
  float accumulator{};
  for (int frame = 0; frame < framesPerSecond * 2; ++frame) {
    accumulator += 1.F / static_cast<float>(framesPerSecond);
    while (accumulator + 0.00001F >= fixed) {
      session.update(fixed, {{1.F, 0.F}, {1.F, 0.F}});
      accumulator -= fixed;
    }
  }
  return session.snapshot();
}
}  // namespace

int main() {
  using namespace isaac::model;
  check(CharacterCatalog::all().size() == 4, "four selectable characters");
  check(CharacterCatalog::at(1).moveSpeed < CharacterCatalog::at(2).moveSpeed, "character stats differ");

  HealthComponent health(12, 10, 5);
  check(health.red() + health.shields() == 12, "combined heart cap");
  health.damage(3);
  check(health.shields() == 0 && health.red() == 9, "shields absorb before red hearts");
  health.damage(20);
  check(health.dead(), "damage causes death");

  GameSession session;
  session.update(1.F / 60.F, {{}, {1.F, 0.F}});
  check(session.projectiles().size() == 1, "shoot creates projectile");
  for (int i = 0; i < 120; ++i) session.update(1.F / 60.F, {{}, {}});
  check(session.projectiles().empty(), "expired projectile is destroyed");

  Level level(42U);
  Inventory inventory;
  check(level.connected() && level.hasRequiredRoomTypes(), "map is connected with required rooms");
  check(!level.enter(4, inventory, false), "secret room rejects entry without bomb");
  check(level.enter(4, inventory, true), "bomb reveals and enters secret room");
  check(inventory.bombs() == 0, "secret reveal consumes bomb");
  check(level.enter(0, inventory, false), "can return from secret room");
  check(level.enter(2, inventory, false), "key opens treasure room");
  check(inventory.keys() == 0, "locked room consumes key");
  check(level.enter(0, inventory, false), "room round trip returns to start");
  check(level.rooms().at(4).visited && level.rooms().at(2).visited, "room state persists after round trips");

  check(EnemyCatalog::all().size() == 6, "six configured normal enemies");
  EnemySystem enemySystem;
  enemySystem.spawnForNormalRoom(1, 1);
  Player testPlayer(CharacterCatalog::at(0));
  std::vector<Projectile> hitProjectiles;
  std::vector<Pickup> drops;
  auto& target = enemySystem.enemies().front();
  target.health = 1.F;
  hitProjectiles.push_back({target.position, {}, 2.F, 1.F, true, true});
  enemySystem.update(0.F, testPlayer, hitProjectiles, drops);
  check(enemySystem.enemies().size() >= 1 && hitProjectiles.empty(), "projectile hit damages and is destroyed");
  while (!enemySystem.enemies().empty()) {
    auto& remaining = enemySystem.enemies().front();
    remaining.health = 1.F;
    hitProjectiles.push_back({remaining.position, {}, 2.F, 1.F, true, true});
    enemySystem.update(0.F, testPlayer, hitProjectiles, drops);
  }
  check(enemySystem.empty(), "enemy damage and death clear encounter");
  check(!drops.empty(), "configured enemy death produces drops");

  ItemSystem itemSystem;
  Player itemPlayer(CharacterCatalog::at(0));
  const float baseDamage = itemPlayer.shooting().damage();
  itemSystem.apply(itemPlayer, ItemCatalog::byId("small_rock"));
  itemSystem.apply(itemPlayer, ItemCatalog::byId("small_rock"));
  check(itemPlayer.shooting().damage() == baseDamage + 2.F, "passive effects stack");
  itemPlayer.inventory().useKey();
  check(!itemSystem.openChest(itemPlayer), "chest fails without a key after resource is spent");
  itemPlayer.inventory().addKeys(1);
  check(itemSystem.openChest(itemPlayer) && itemPlayer.inventory().keys() == 0, "chest consumes key and grants item");
  check(!itemSystem.buyShopItem(itemPlayer), "shop rejects insufficient coins");
  itemPlayer.inventory().addCoins(7);
  check(itemSystem.buyShopItem(itemPlayer) && itemPlayer.inventory().coins() == 2, "shop purchase is atomic");
  check(itemSystem.takeSecretTrinket(itemPlayer) && itemPlayer.luck() > 0.F, "secret trinket applies luck");

  Inventory economy;
  check(!economy.spendCoins(1), "coin consume fails when empty");
  economy.addCoins(2);
  check(economy.spendCoins(2) && economy.coins() == 0, "coin consume succeeds atomically");
  check(economy.useBomb() && !economy.useBomb(), "bomb success and failure consumption");
  check(economy.useKey() && !economy.useKey(), "key success and failure consumption");

  const auto at30 = simulateAtRenderRate(30);
  const auto at60 = simulateAtRenderRate(60);
  const auto at120 = simulateAtRenderRate(120);
  check(std::abs(at30.playerPosition.x - at60.playerPosition.x) < 0.1F &&
        std::abs(at60.playerPosition.x - at120.playerPosition.x) < 0.1F,
        "fixed-step movement is render-rate independent");
  check(at30.totalShots == at60.totalShots && at60.totalShots == at120.totalShots,
        "fixed-step shooting is render-rate independent");
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
