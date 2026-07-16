#include "model/CharacterCatalog.h"
#include "model/Components.h"
#include "model/GameSession.h"
#include "model/Level.h"
#include "model/EnemySystem.h"
#include "model/ItemSystem.h"
#include "model/BossSystem.h"

#include <algorithm>
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

void runCombatUntilClear(isaac::model::GameSession& session, int maxFrames = 5000) {
  for (int frame = 0; frame < maxFrames && !session.snapshot().roomCleared; ++frame) {
    isaac::model::GameplayInput input;
    if (!session.snapshot().enemies.empty()) {
      input.shooting = session.snapshot().enemies.front().position - session.snapshot().playerPosition;
    }
    session.update(1.F / 60.F, input);
  }
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

  Player damagePlayer(CharacterCatalog::at(0));
  check(damagePlayer.damage(1), "Player reports damage that changes health");
  check(!damagePlayer.damage(1), "Player rejects damage during invulnerability");

  GameSession session;
  const auto& startDoors = session.snapshot().doors;
  check(startDoors.size() == 4, "start room snapshot exposes every modeled exit");
  const auto treasureDoor = std::ranges::find_if(startDoors, [](const DoorSnapshot& door) {
    return door.targetType == isaac::common::RoomType::Treasure;
  });
  check(treasureDoor != startDoors.end() && treasureDoor->locked && !treasureDoor->hidden &&
            !treasureDoor->sealed,
        "treasure door snapshot exposes locked visible open-state details");
  const auto secretDoor = std::ranges::find_if(startDoors, [](const DoorSnapshot& door) {
    return door.targetType == isaac::common::RoomType::Secret;
  });
  check(secretDoor != startDoors.end() && secretDoor->hidden,
        "secret entrance remains hidden in the Model snapshot until revealed");
  session.update(1.F / 60.F, {{}, {1.F, 0.F}});
  check(session.projectiles().size() == 1, "shoot creates projectile");
  const auto shotEvents = session.drainEvents();
  check(std::ranges::find(shotEvents, ModelEvent::Shot) != shotEvents.end(),
        "shoot records a Model event at the behavior source");
  check(session.drainEvents().empty(), "Model events drain exactly once");
  GameSession resetEventSession;
  resetEventSession.update(1.F / 60.F, {{}, {1.F, 0.F}});
  resetEventSession.selectCharacter(1);
  check(resetEventSession.drainEvents().empty(),
        "selecting a character clears events from the previous run");
  for (int i = 0; i < 120; ++i) session.update(1.F / 60.F, {{}, {}});
  check(session.projectiles().empty(), "expired projectile is destroyed");

  GameSession hurtSession;
  for (int frame = 0; frame < 220 && hurtSession.level().currentRoomId() == 0; ++frame) {
    hurtSession.update(1.F / 60.F, {{1.F, 0.F}, {}});
  }
  (void)hurtSession.drainEvents();
  hurtSession.player().resetInvulnerability();
  hurtSession.enemySystem().enemies().front().position = hurtSession.player().position();
  hurtSession.update(0.F, {});
  const auto hurtEvents = hurtSession.drainEvents();
  check(std::ranges::find(hurtEvents, ModelEvent::Hurt) != hurtEvents.end(),
        "effective enemy damage records a Hurt event");
  hurtSession.update(0.F, {});
  const auto blockedHurtEvents = hurtSession.drainEvents();
  check(std::ranges::find(blockedHurtEvents, ModelEvent::Hurt) == blockedHurtEvents.end(),
        "invulnerability prevents a false Hurt event");

  GameSession pickupSession;
  for (int frame = 0; frame < 220 && pickupSession.level().currentRoomId() == 0; ++frame) {
    pickupSession.update(1.F / 60.F, {{1.F, 0.F}, {}});
  }
  int expectedPickups{};
  for (auto& enemy : pickupSession.enemySystem().enemies()) {
    if (EnemyCatalog::all()[enemy.definitionIndex].drop != DropStrategy::None) ++expectedPickups;
    enemy.position = pickupSession.player().position();
    enemy.health = 0.F;
  }
  (void)pickupSession.drainEvents();
  pickupSession.update(0.F, {});
  const auto pickupEvents = pickupSession.drainEvents();
  check(std::ranges::count(pickupEvents, ModelEvent::Pickup) == expectedPickups,
        "each collected drop records one Pickup event at its behavior source");
  check(pickupSession.snapshot().pickups.empty(),
        "same-tick generated and collected pickups leave no snapshot entity");

  Level level(42U);
  Inventory inventory;
  check(level.connected() && level.hasRequiredRoomTypes(), "map is connected with required rooms");
  check(!level.enter(4, inventory, false), "secret room rejects entry without bomb");
  check(level.enter(4, inventory, true), "bomb reveals and enters secret room");
  check(inventory.bombs() == 0, "secret reveal consumes bomb");
  check(level.enter(0, inventory, false), "can return from secret room");
  check(level.enter(2, inventory, false), "key opens treasure room");
  check(inventory.keys() == 1, "locked room consumes exactly one key");
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
  check(economy.useKey() && economy.useKey() && !economy.useKey(), "key success and failure consumption");

  check(BossCatalog::all().size() == 4, "four configured bosses");
  check(BossCatalog::all().back().id == "moms_leg", "final boss is simplified Mom's Leg");
  BossSystem bossSystem;
  bossSystem.spawnForFloor(2);
  check(bossSystem.bosses().size() == 2, "second floor exposes two distinct bosses");
  for (auto& boss : bossSystem.bosses()) boss.health = 0.F;
  std::vector<Projectile> bossProjectiles;
  bossSystem.update(0.F, testPlayer, bossProjectiles);
  check(bossSystem.empty(), "boss death resolves encounter");
  check(DevilRoomPolicy::opens(0.F) && DevilRoomPolicy::opens(0.349F) &&
        !DevilRoomPolicy::opens(0.35F) && !DevilRoomPolicy::opens(0.99F),
        "devil room probability boundaries are deterministic");

  Level progression(7U);
  Inventory progressionInventory;
  check(progression.enter(1, progressionInventory, false), "enter normal route to boss");
  progression.markCurrentCleared();
  check(progression.enter(5, progressionInventory, false), "enter boss room after clear");
  progression.markCurrentCleared();
  check(progression.addDevilRoom(), "boss clear can add devil room");
  check(progression.advanceFloor() && progression.floorNumber() == 2, "boss death advances floor");

  GameSession integratedRoom;
  for (int i = 0; i < 220 && integratedRoom.level().currentRoomId() == 0; ++i) {
    integratedRoom.update(1.F / 60.F, {{1.F, 0.F}, {}});
  }
  check(integratedRoom.level().currentRoomId() == 1 && !integratedRoom.snapshot().roomCleared &&
        !integratedRoom.snapshot().enemies.empty(), "GameSession enters a populated uncleared room");
  check(std::ranges::all_of(integratedRoom.snapshot().doors,
                            [](const DoorSnapshot& door) { return door.sealed; }),
        "all combat-room exits report sealed until the encounter is cleared");
  for (int i = 0; i < 300; ++i) integratedRoom.update(1.F / 60.F, {{1.F, 0.F}, {}});
  check(integratedRoom.level().currentRoomId() == 1, "uncleared GameSession room blocks the boss door");
  runCombatUntilClear(integratedRoom);
  check(integratedRoom.snapshot().roomCleared && integratedRoom.snapshot().enemies.empty(),
        "real projectile combat clears the GameSession room and opens doors");
  check(std::ranges::none_of(integratedRoom.snapshot().doors,
                             [](const DoorSnapshot& door) { return door.sealed; }),
        "clearing combat updates every exit to unsealed");
  for (int i = 0; i < 300 && integratedRoom.level().currentRoomId() == 1; ++i) {
    integratedRoom.update(1.F / 60.F, {{1.F, 0.F}, {}});
  }
  check(integratedRoom.level().currentRoomId() == 5 && !integratedRoom.snapshot().bosses.empty(),
        "cleared GameSession room permits transition and spawns the boss");

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
