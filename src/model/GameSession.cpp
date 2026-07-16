#include "model/GameSession.h"

#include "model/CharacterCatalog.h"

#include <algorithm>
#include <array>
#include <random>
#include <string_view>
#include <utility>

namespace {

constexpr float PlayerRadius = 18.F;
constexpr float ProjectileRadius = 6.F;
constexpr isaac::common::Vec2 TreasureItemPosition{480.F, 300.F};
constexpr std::array<isaac::model::ObstacleSnapshot, 2> RoomObstacles{{
    {isaac::common::ObstacleType::Rock, {315.F, 270.F}, 28.F},
    {isaac::common::ObstacleType::Trap, {710.F, 430.F}, 20.F},
}};

bool overlaps(isaac::common::Vec2 first, float firstRadius,
              isaac::common::Vec2 second, float secondRadius) {
  const float combinedRadius = firstRadius + secondRadius;
  return (first - second).lengthSquared() < combinedRadius * combinedRadius;
}

bool overlapsRock(isaac::common::Vec2 position, float radius) {
  return std::ranges::any_of(RoomObstacles, [position, radius](const auto& obstacle) {
    return obstacle.type == isaac::common::ObstacleType::Rock &&
           overlaps(position, radius, obstacle.position, obstacle.radius);
  });
}

isaac::common::Vec2 resolveRockOverlap(isaac::common::Vec2 position, float radius) {
  for (const auto& obstacle : RoomObstacles) {
    if (obstacle.type != isaac::common::ObstacleType::Rock ||
        !overlaps(position, radius, obstacle.position, obstacle.radius)) continue;
    auto direction = (position - obstacle.position).normalized();
    if (direction.lengthSquared() < 0.1F) direction = {1.F, 0.F};
    position = obstacle.position + direction * (radius + obstacle.radius);
  }
  return position;
}

}  // namespace

namespace isaac::model {

GameSession::GameSession(float devilRoomRoll, unsigned treasureSeed)
    : player_(CharacterCatalog::at(0)), level_(1337U), devilRoomRoll_(devilRoomRoll),
      treasureRng_(treasureSeed == 0U ? std::random_device{}() : treasureSeed) {
  chooseTreasureItem();
  rebuildSnapshot();
}

void GameSession::chooseTreasureItem() {
  static constexpr std::array<std::string_view, 3> rewards{
      "breakfast", "wiggle_worm", "sad_onion"};
  std::uniform_int_distribution<std::size_t> choose(0, rewards.size() - 1);
  treasureItemId_ = rewards[choose(treasureRng_)];
}

void GameSession::selectCharacter(std::size_t index) {
  player_ = Player(CharacterCatalog::at(index));
  level_ = Level(1337U);
  enemies_ = EnemySystem{};
  pickups_.clear();
  items_ = ItemSystem{};
  bosses_ = BossSystem{};
  bossEncounterActive_ = false;
  bossRewardResolved_ = false;
  runCompleted_ = false;
  projectiles_.clear();
  snapshot_ = SessionSnapshot{};
  pendingEvents_.clear();
  chooseTreasureItem();
  rebuildSnapshot();
}

std::size_t GameSession::selectableCharacterCount() const {
  return CharacterCatalog::all().size();
}

CharacterSummary GameSession::selectableCharacter(std::size_t index) const {
  const auto& character = CharacterCatalog::at(index);
  return {std::string(character.id), std::string(character.displayName), character.redHearts,
          character.moveSpeed, character.damage, character.luck};
}

std::vector<ModelEvent> GameSession::drainEvents() {
  auto events = std::move(pendingEvents_);
  pendingEvents_.clear();
  return events;
}

void GameSession::update(float seconds, const GameplayInput& input) {
  seconds = std::clamp(seconds, 0.F, 0.1F);
  snapshot_.elapsedSeconds += seconds;
  player_.tick(seconds);
  player_.move(input.movement, seconds);
  player_.setPosition(resolveRockOverlap(player_.position(), PlayerRadius));

  const bool hurtByTrap = std::ranges::any_of(RoomObstacles, [this](const auto& obstacle) {
    return obstacle.type == common::ObstacleType::Trap &&
           overlaps(player_.position(), PlayerRadius, obstacle.position, obstacle.radius) &&
           player_.damageHalfHeart();
  });

  std::optional<common::Direction> exitDirection;
  if (player_.position().x <= 60.1F && input.movement.x < 0.F) exitDirection = common::Direction::Left;
  if (player_.position().x >= 899.9F && input.movement.x > 0.F) exitDirection = common::Direction::Right;
  if (player_.position().y <= 100.1F && input.movement.y < 0.F) exitDirection = common::Direction::Up;
  if (player_.position().y >= 479.9F && input.movement.y > 0.F) exitDirection = common::Direction::Down;
  if (exitDirection) {
    if (const auto next = level_.neighbor(*exitDirection); next && level_.enter(*next, player_.inventory(), input.useBomb)) {
      common::Vec2 arrival = player_.position();
      if (*exitDirection == common::Direction::Left) arrival.x = 890.F;
      if (*exitDirection == common::Direction::Right) arrival.x = 70.F;
      if (*exitDirection == common::Direction::Up) arrival.y = 470.F;
      if (*exitDirection == common::Direction::Down) arrival.y = 110.F;
      player_.setPosition(arrival);
      projectiles_.clear();
      pickups_.clear();
      if (level_.currentRoom().type == common::RoomType::Normal && !level_.currentRoom().cleared) {
        enemies_.spawnForNormalRoom(level_.currentRoomId(), level_.floorNumber());
      }
      if (level_.currentRoom().type == common::RoomType::Boss && !level_.currentRoom().cleared) {
        bosses_.spawnForFloor(level_.floorNumber());
        bossEncounterActive_ = true;
        bossRewardResolved_ = false;
      }
    }
  }

  if (input.shooting.lengthSquared() > 0.1F && player_.canShoot()) {
    const auto direction = input.shooting.normalized();
    projectiles_.push_back({player_.position(), direction * player_.shooting().projectileSpeed(),
                            player_.shooting().damage(), 1.6F, true, true,
                            player_.shooting().sineProjectiles()});
    ++snapshot_.totalShots;
    pendingEvents_.push_back(ModelEvent::Shot);
  }
  for (auto& projectile : projectiles_) projectile.update(seconds);
  std::erase_if(projectiles_, [](const Projectile& projectile) {
    return !projectile.alive || overlapsRock(projectile.position, ProjectileRadius);
  });
  const bool hurtByEnemy = enemies_.update(seconds, player_, projectiles_, pickups_);
  const bool hurtByBoss = bosses_.update(seconds, player_, projectiles_);
  if (hurtByTrap || hurtByEnemy || hurtByBoss) {
    pendingEvents_.push_back(ModelEvent::Hurt);
  }
  if (level_.currentRoom().type == common::RoomType::Normal &&
      !level_.currentRoom().cleared && enemies_.empty()) {
    level_.markCurrentCleared();
  }
  if (bossEncounterActive_ && bosses_.empty()) {
    bossEncounterActive_ = false;
    level_.markCurrentCleared();
    if (!bossRewardResolved_ && DevilRoomPolicy::opens(devilRoomRoll_)) level_.addDevilRoom();
    bossRewardResolved_ = true;
  }
  for (const auto& pickup : pickups_) {
    if ((pickup.position - player_.position()).lengthSquared() < 26.F * 26.F) {
      if (pickup.type == common::PickupType::Coin) player_.inventory().addCoins(1);
      if (pickup.type == common::PickupType::Bomb) player_.inventory().addBombs(1);
      if (pickup.type == common::PickupType::Key) player_.inventory().addKeys(1);
      pendingEvents_.push_back(ModelEvent::Pickup);
    }
  }
  std::erase_if(pickups_, [this](const Pickup& pickup) {
    return (pickup.position - player_.position()).lengthSquared() < 26.F * 26.F;
  });
  if (level_.currentRoom().type == common::RoomType::Treasure && !items_.treasureTaken() &&
      overlaps(player_.position(), PlayerRadius, TreasureItemPosition, 18.F) &&
      items_.takeTreasureItem(player_, treasureItemId_)) {
    pendingEvents_.push_back(ModelEvent::Pickup);
  }
  if (input.useActive) items_.useActive(player_);
  if (input.interact && level_.currentRoom().cleared) {
    if (level_.currentRoom().type == common::RoomType::Shop) items_.buyShopItem(player_);
    if (level_.currentRoom().type == common::RoomType::Boss) {
      if (level_.floorNumber() == Level::MaxFloors) {
        runCompleted_ = true;
      } else if (level_.advanceFloor()) {
        player_.setPosition({480.F, 300.F});
        projectiles_.clear(); pickups_.clear(); enemies_ = EnemySystem{}; bosses_ = BossSystem{};
        items_ = ItemSystem{};
        bossRewardResolved_ = false;
        chooseTreasureItem();
      }
    }
  }
  if (level_.currentRoom().type == common::RoomType::Secret) items_.takeSecretTrinket(player_);
  rebuildSnapshot();
}

void GameSession::rebuildSnapshot() {
  snapshot_.playerPosition = player_.position();
  snapshot_.characterId = std::string(player_.definition().id);
  snapshot_.heartContainers = player_.health().containers();
  snapshot_.redHearts = player_.health().redHalfUnits() / 2;
  snapshot_.redHalfHeart = player_.health().redHalfUnits() % 2 != 0;
  snapshot_.shields = player_.health().shieldHalfUnits() / 2;
  snapshot_.shieldHalfHeart = player_.health().shieldHalfUnits() % 2 != 0;
  snapshot_.coins = player_.inventory().coins();
  snapshot_.bombs = player_.inventory().bombs();
  snapshot_.keys = player_.inventory().keys();
  snapshot_.activeItem = player_.inventory().activeItem();
  snapshot_.moveSpeed = player_.definition().moveSpeed;
  snapshot_.damage = player_.shooting().damage();
  snapshot_.shotsPerSecond = player_.shooting().shotsPerSecond();
  snapshot_.projectileSpeed = player_.shooting().projectileSpeed();
  snapshot_.playerDead = player_.health().dead();
  snapshot_.runCompleted = runCompleted_;
  snapshot_.projectiles.clear();
  for (const auto& projectile : projectiles_) {
    snapshot_.projectiles.push_back({projectile.position, projectile.friendly});
  }
  snapshot_.floor = level_.floorNumber();
  snapshot_.roomType = level_.currentRoom().type;
  snapshot_.roomCleared = level_.currentRoom().cleared;
  snapshot_.rooms.clear();
  for (const auto& room : level_.rooms()) {
    snapshot_.rooms.push_back({room.id, room.type, room.gridX, room.gridY, room.visited,
                               room.revealed, room.id == level_.currentRoomId(), room.cleared});
  }
  snapshot_.doors.clear();
  constexpr std::array directions{common::Direction::Up, common::Direction::Right,
                                  common::Direction::Down, common::Direction::Left};
  for (const auto direction : directions) {
    const auto neighbor = level_.neighbor(direction);
    if (!neighbor) continue;
    const auto& target = level_.rooms().at(*neighbor);
    snapshot_.doors.push_back(
        {direction, target.type, target.locked, !target.revealed, !level_.currentRoom().cleared});
  }
  snapshot_.enemies.clear();
  for (const auto& enemy : enemies_.enemies()) {
    snapshot_.enemies.push_back({enemy.position, std::string(EnemyCatalog::all()[enemy.definitionIndex].id)});
  }
  snapshot_.pickups.clear();
  for (const auto& pickup : pickups_) snapshot_.pickups.push_back({pickup.position, pickup.type});
  snapshot_.bosses.clear();
  for (const auto& boss : bosses_.bosses()) {
    snapshot_.bosses.push_back({boss.position, std::string(BossCatalog::all()[boss.definitionIndex].id), boss.phase});
  }
  snapshot_.obstacles.assign(RoomObstacles.begin(), RoomObstacles.end());
  snapshot_.treasureItems.clear();
  if (level_.currentRoom().type == common::RoomType::Treasure && !items_.treasureTaken()) {
    snapshot_.treasureItems.push_back({TreasureItemPosition, treasureItemId_});
  }
  snapshot_.devilRoomAvailable = std::ranges::any_of(level_.rooms(), [](const Room& room) {
    return room.type == common::RoomType::Devil;
  });
  snapshot_.roomRewardCollected =
      (level_.currentRoom().type == common::RoomType::Treasure && items_.treasureTaken()) ||
      (level_.currentRoom().type == common::RoomType::Shop && items_.shopSold()) ||
      (level_.currentRoom().type == common::RoomType::Secret && items_.secretTaken());
}

}  // namespace isaac::model
