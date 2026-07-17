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
#include <string>

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
      const auto toward =
          (session.snapshot().enemies.front().position - session.snapshot().playerPosition).normalized();
      input.shooting = toward;
      input.movement = {-toward.y, toward.x};
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
  check(startDoors.size() == 2, "start room exposes only the monster and treasure exits");
  const auto treasureDoor = std::ranges::find_if(startDoors, [](const DoorSnapshot& door) {
    return door.targetType == isaac::common::RoomType::Treasure;
  });
  check(treasureDoor != startDoors.end() && treasureDoor->locked && !treasureDoor->hidden &&
            !treasureDoor->sealed,
        "treasure door snapshot exposes locked visible open-state details");
  check(std::ranges::none_of(startDoors, [](const DoorSnapshot& door) {
          return door.targetType == isaac::common::RoomType::Shop ||
                 door.targetType == isaac::common::RoomType::Secret ||
                 door.targetType == isaac::common::RoomType::Devil;
        }),
        "simplified start room has no shop, secret, or devil exit");
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

  GameSession restartSession;
  check(restartSession.level().enter(1, restartSession.player().inventory(), false),
        "restart regression enters the combat route");
  restartSession.level().markCurrentCleared();
  check(restartSession.level().enter(3, restartSession.player().inventory(), false),
        "restart regression enters the boss route");
  restartSession.level().markCurrentCleared();
  check(restartSession.level().advanceFloor(), "restart regression reaches floor two");
  restartSession.player().setPosition({800.F, 420.F});
  restartSession.player().damage(20);
  restartSession.selectCharacter(2);
  check(restartSession.snapshot().floor == 1 && restartSession.level().currentRoomId() == 0 &&
            (restartSession.snapshot().playerPosition - isaac::common::Vec2{480.F, 300.F}).lengthSquared() < 0.01F &&
            !restartSession.snapshot().playerDead && restartSession.snapshot().totalShots == 0 &&
            restartSession.snapshot().elapsedSeconds == 0.F,
        "character selection resets the complete run after death");

  GameSession obstacleSession;
  const auto rock = std::ranges::find_if(obstacleSession.snapshot().obstacles, [](const auto& obstacle) {
    return obstacle.type == isaac::common::ObstacleType::Rock;
  });
  const auto trap = std::ranges::find_if(obstacleSession.snapshot().obstacles, [](const auto& obstacle) {
    return obstacle.type == isaac::common::ObstacleType::Trap;
  });
  const bool hasRock = rock != obstacleSession.snapshot().obstacles.end();
  const bool hasTrap = trap != obstacleSession.snapshot().obstacles.end();
  check(obstacleSession.snapshot().obstacles.size() == 2 &&
            hasRock && hasTrap,
        "room snapshot contains one physical rock and one damaging trap");
  const auto rockPosition = hasRock ? rock->position : isaac::common::Vec2{};
  const float rockRadius = hasRock ? rock->radius : 0.F;
  const auto trapPosition = hasTrap ? trap->position : isaac::common::Vec2{};
  if (hasRock) {
    const float contactX = rockPosition.x - rockRadius - 18.F;
    obstacleSession.player().setPosition({contactX - 2.F, rockPosition.y});
    for (int frame = 0; frame < 30; ++frame) {
      obstacleSession.update(1.F / 60.F, {{1.F, 0.F}, {}});
    }
    check(obstacleSession.snapshot().playerPosition.x <= contactX + 0.1F,
          "rock volume blocks player movement");

    GameSession blockedShotSession;
    blockedShotSession.player().setPosition({rockPosition.x - 100.F, rockPosition.y});
    blockedShotSession.update(1.F / 60.F, {{}, {1.F, 0.F}});
    for (int frame = 0; frame < 30; ++frame) blockedShotSession.update(1.F / 60.F, {});
    check(blockedShotSession.projectiles().empty(), "rock volume destroys colliding projectiles");
  }
  if (hasTrap) {
    GameSession trapSession;
    trapSession.player().setPosition(trapPosition);
    const int heartsBefore = trapSession.player().health().totalHalfUnits();
    trapSession.update(0.F, {});
    const int heartsAfter = trapSession.player().health().totalHalfUnits();
    check(heartsAfter == heartsBefore - 1, "trap contact removes exactly half a heart");
    check(trapSession.snapshot().redHearts == 2 && trapSession.snapshot().redHalfHeart,
          "half-heart trap damage is preserved in the presentation snapshot");
    const auto trapEvents = trapSession.drainEvents();
    check(std::ranges::find(trapEvents, ModelEvent::Hurt) != trapEvents.end(),
          "trap damage emits the standard Hurt event");
  }

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
  pickupSession.player().damage(1);
  const int containersBeforeDrops = pickupSession.player().health().containers();
  const int redHalfUnitsBeforeDrops = pickupSession.player().health().redHalfUnits();
  int expectedPickups{};
  for (auto& enemy : pickupSession.enemySystem().enemies()) {
    if (EnemyCatalog::all()[enemy.definitionIndex].drop == DropStrategy::Breakfast) ++expectedPickups;
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
  check(expectedPickups > 0 &&
            pickupSession.player().health().containers() == containersBeforeDrops + expectedPickups &&
            pickupSession.player().health().redHalfUnits() ==
                redHalfUnitsBeforeDrops + expectedPickups * 2,
        "monster-dropped Breakfast adds one container and heals one whole heart");

  Level level;
  Inventory inventory;
  const auto roomCount = [&level](isaac::common::RoomType type) {
    return std::ranges::count_if(level.rooms(), [type](const Room& room) {
      return room.type == type;
    });
  };
  check(level.connected() && level.hasRequiredRoomTypes() && level.rooms().size() == 4 &&
            roomCount(isaac::common::RoomType::Normal) == 2 &&
            roomCount(isaac::common::RoomType::Treasure) == 1 &&
            roomCount(isaac::common::RoomType::Boss) == 1 &&
            roomCount(isaac::common::RoomType::Shop) == 0 &&
            roomCount(isaac::common::RoomType::Secret) == 0 &&
            roomCount(isaac::common::RoomType::Devil) == 0,
        "each floor has exactly one start, monster, treasure, and boss room");
  check(level.enter(2, inventory, false), "key opens treasure room");
  check(inventory.keys() == 1, "locked room consumes exactly one key");
  check(level.enter(0, inventory, false), "room round trip returns to start");
  check(level.rooms().at(2).visited, "treasure-room state persists after a round trip");

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
  check(!drops.empty() && std::ranges::all_of(drops, [](const Pickup& pickup) {
          return pickup.type == isaac::common::PickupType::Passive;
        }),
        "configured monster drops are typed as Breakfast passives");

  ItemSystem itemSystem;
  Player itemPlayer(CharacterCatalog::at(0));
  const float baseDamage = itemPlayer.shooting().damage();
  itemSystem.apply(itemPlayer, ItemCatalog::byId("small_rock"));
  itemSystem.apply(itemPlayer, ItemCatalog::byId("small_rock"));
  check(itemPlayer.shooting().damage() == baseDamage + 2.F, "passive effects stack");
  Player breakfastPlayer(CharacterCatalog::at(0));
  breakfastPlayer.damage(1);
  const int redHalfUnitsBeforeBreakfast = breakfastPlayer.health().redHalfUnits();
  const int containersBeforeBreakfast = breakfastPlayer.health().containers();
  itemSystem.apply(breakfastPlayer, ItemCatalog::byId("breakfast"));
  check(breakfastPlayer.health().redHalfUnits() == redHalfUnitsBeforeBreakfast + 2 &&
            breakfastPlayer.health().containers() == containersBeforeBreakfast + 1,
        "Breakfast adds a heart container and restores one whole heart");
  Player wigglePlayer(CharacterCatalog::at(0));
  const float baseShotsPerSecond = wigglePlayer.shooting().shotsPerSecond();
  itemSystem.apply(wigglePlayer, ItemCatalog::byId("wiggle_worm"));
  check(wigglePlayer.shooting().sineProjectiles() &&
            std::abs(wigglePlayer.shooting().shotsPerSecond() - baseShotsPerSecond) < 0.001F,
        "Wiggle Worm enables sine tears without changing fire rate");
  Player onionPlayer(CharacterCatalog::at(0));
  const float onionBaseShotsPerSecond = onionPlayer.shooting().shotsPerSecond();
  itemSystem.apply(onionPlayer, ItemCatalog::byId("sad_onion"));
  check(!onionPlayer.shooting().sineProjectiles() &&
            onionPlayer.shooting().shotsPerSecond() > onionBaseShotsPerSecond,
        "Sad Onion raises fire rate without changing projectile trajectory");
  Projectile sineProjectile{{100.F, 200.F}, {100.F, 0.F}, 1.F, 1.F, true, true, true};
  sineProjectile.update(0.1F);
  check(sineProjectile.position.x > 100.F && std::abs(sineProjectile.position.y - 200.F) > 0.1F,
        "sine projectile follows a curved trajectory");
  const int keysBeforeTreasure = itemPlayer.inventory().keys();
  check(!itemSystem.takeTreasureItem(itemPlayer, "breakfast"),
        "Breakfast is reserved for monster drops instead of treasure rooms");
  check(itemSystem.takeTreasureItem(itemPlayer, "wiggle_worm") &&
            itemPlayer.inventory().keys() == keysBeforeTreasure,
        "treasure pickup grants an allowed item without charging a second key");
  check(!itemSystem.takeTreasureItem(itemPlayer, "sad_onion"),
        "treasure room grants at most one item");
  ItemSystem onionTreasureSystem;
  Player onionTreasurePlayer(CharacterCatalog::at(0));
  check(onionTreasureSystem.takeTreasureItem(onionTreasurePlayer, "sad_onion"),
        "fire-rate item is a valid treasure-room reward");
  check(!itemSystem.buyShopItem(itemPlayer), "shop rejects insufficient coins");
  itemPlayer.inventory().addCoins(7);
  check(itemSystem.buyShopItem(itemPlayer) && itemPlayer.inventory().coins() == 2, "shop purchase is atomic");
  check(itemSystem.takeSecretTrinket(itemPlayer) && itemPlayer.luck() > 0.F, "secret trinket applies luck");

  GameSession treasureSession(7U);
  check(treasureSession.level().enter(2, treasureSession.player().inventory(), false),
        "treasure room opens through its modeled locked door");
  treasureSession.player().setPosition({890.F, 300.F});
  treasureSession.update(0.F, {});
  check(treasureSession.snapshot().treasureItems.size() == 1,
        "an unclaimed treasure room contains exactly one visible item");
  std::string firstFloorTreasureId;
  if (!treasureSession.snapshot().treasureItems.empty()) {
    const auto treasure = treasureSession.snapshot().treasureItems.front();
    firstFloorTreasureId = treasure.id;
    check(treasure.id == "wiggle_worm" || treasure.id == "sad_onion",
          "treasure reward is selected from Wiggle Worm and Sad Onion only");
    treasureSession.player().setPosition(treasure.position);
    treasureSession.update(0.F, {});
    check(treasureSession.snapshot().treasureItems.empty() &&
              treasureSession.snapshot().roomRewardCollected &&
              treasureSession.player().inventory().passiveItems().size() == 1,
          "touching the treasure item collects it and removes it from the room");
  }
  check(treasureSession.level().enter(0, treasureSession.player().inventory(), false) &&
            treasureSession.level().enter(1, treasureSession.player().inventory(), false),
        "treasure regression returns to the floor route");
  treasureSession.level().markCurrentCleared();
  check(treasureSession.level().enter(3, treasureSession.player().inventory(), false),
        "treasure regression reaches the floor-one boss");
  treasureSession.level().markCurrentCleared();
  GameplayInput advanceFloor;
  advanceFloor.interact = true;
  treasureSession.update(0.F, advanceFloor);
  check(treasureSession.level().floorNumber() == 2 &&
            treasureSession.level().enter(2, treasureSession.player().inventory(), false),
        "second floor exposes its treasure room");
  treasureSession.player().setPosition({890.F, 300.F});
  treasureSession.update(0.F, {});
  check(treasureSession.snapshot().treasureItems.size() == 1 &&
            !firstFloorTreasureId.empty() &&
            treasureSession.snapshot().treasureItems.front().id != firstFloorTreasureId,
        "the second-floor treasure is present and cannot repeat the first-floor reward");

  bool sawWiggleWorm{};
  bool sawSadOnion{};
  for (unsigned seed = 1; seed <= 96; ++seed) {
    GameSession seededTreasureSession(seed);
    if (!seededTreasureSession.level().enter(
            2, seededTreasureSession.player().inventory(), false)) continue;
    seededTreasureSession.player().setPosition({890.F, 300.F});
    seededTreasureSession.update(0.F, {});
    if (seededTreasureSession.snapshot().treasureItems.empty()) continue;
    const auto& id = seededTreasureSession.snapshot().treasureItems.front().id;
    sawWiggleWorm = sawWiggleWorm || id == "wiggle_worm";
    sawSadOnion = sawSadOnion || id == "sad_onion";
  }
  check(sawWiggleWorm && sawSadOnion,
        "seeded treasure-room selection reaches both allowed rewards and no Breakfast");

  Inventory economy;
  check(!economy.spendCoins(1), "coin consume fails when empty");
  economy.addCoins(2);
  check(economy.spendCoins(2) && economy.coins() == 0, "coin consume succeeds atomically");
  check(economy.useBomb() && !economy.useBomb(), "bomb success and failure consumption");
  check(economy.useKey() && economy.useKey() && !economy.useKey(), "key success and failure consumption");

  check(BossCatalog::all().size() == 3, "three configured bosses remain in the two-floor run");
  check(BossCatalog::all().back().id == "larry_jr", "second-floor duo completes the Boss catalog");
  BossSystem bossSystem;
  bossSystem.spawnForFloor(2);
  check(bossSystem.bosses().size() == 2, "second floor exposes two distinct bosses");
  for (auto& boss : bossSystem.bosses()) boss.health = 0.F;
  std::vector<Projectile> bossProjectiles;
  bossSystem.update(0.F, testPlayer, bossProjectiles);
  check(bossSystem.empty(), "boss death resolves encounter");
  Level progression;
  Inventory progressionInventory;
  check(progression.enter(1, progressionInventory, false), "enter normal route to boss");
  progression.markCurrentCleared();
  check(progression.enter(3, progressionInventory, false), "enter boss room after clear");
  progression.markCurrentCleared();
  check(progression.advanceFloor() && progression.floorNumber() == 2, "boss death advances floor");
  check(progression.rooms().size() == 4,
        "advancing floors preserves the exact four-room layout");
  check(progression.enter(1, progressionInventory, false), "enter second-floor normal route");
  progression.markCurrentCleared();
  check(progression.enter(3, progressionInventory, false), "enter second-floor boss room");
  progression.markCurrentCleared();
  check(!progression.advanceFloor() && progression.floorNumber() == 2,
        "second floor is final and a third floor cannot be generated");

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
  check(integratedRoom.level().currentRoomId() == 3 && !integratedRoom.snapshot().bosses.empty(),
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
