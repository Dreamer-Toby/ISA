#include "model/GameSession.h"

#include "model/CharacterCatalog.h"

#include <algorithm>

namespace isaac::model {

GameSession::GameSession() : player_(CharacterCatalog::at(0)), level_(1337U) { rebuildSnapshot(); }

void GameSession::selectCharacter(std::size_t index) {
  player_ = Player(CharacterCatalog::at(index));
  projectiles_.clear();
  snapshot_.totalShots = 0;
  snapshot_.elapsedSeconds = 0.F;
  rebuildSnapshot();
}

void GameSession::update(float seconds, const GameplayInput& input) {
  seconds = std::clamp(seconds, 0.F, 0.1F);
  snapshot_.elapsedSeconds += seconds;
  player_.tick(seconds);
  player_.move(input.movement, seconds);

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
    }
  }

  if (input.shooting.lengthSquared() > 0.1F && player_.canShoot()) {
    const auto direction = input.shooting.normalized();
    projectiles_.push_back({player_.position(), direction * player_.shooting().projectileSpeed(),
                            player_.shooting().damage(), 1.6F, true, true});
    ++snapshot_.totalShots;
  }
  for (auto& projectile : projectiles_) projectile.update(seconds);
  std::erase_if(projectiles_, [](const Projectile& projectile) { return !projectile.alive; });
  enemies_.update(seconds, player_, projectiles_, pickups_);
  if (level_.currentRoom().type == common::RoomType::Normal &&
      !level_.currentRoom().cleared && enemies_.empty()) {
    level_.markCurrentCleared();
  }
  for (const auto& pickup : pickups_) {
    if ((pickup.position - player_.position()).lengthSquared() < 26.F * 26.F) {
      if (pickup.type == common::PickupType::Coin) player_.inventory().addCoins(1);
      if (pickup.type == common::PickupType::Bomb) player_.inventory().addBombs(1);
      if (pickup.type == common::PickupType::Key) player_.inventory().addKeys(1);
    }
  }
  std::erase_if(pickups_, [this](const Pickup& pickup) {
    return (pickup.position - player_.position()).lengthSquared() < 26.F * 26.F;
  });
  rebuildSnapshot();
}

void GameSession::rebuildSnapshot() {
  snapshot_.playerPosition = player_.position();
  snapshot_.characterId = std::string(player_.definition().id);
  snapshot_.redHearts = player_.health().red();
  snapshot_.shields = player_.health().shields();
  snapshot_.coins = player_.inventory().coins();
  snapshot_.bombs = player_.inventory().bombs();
  snapshot_.keys = player_.inventory().keys();
  snapshot_.activeItem = player_.inventory().activeItem();
  snapshot_.playerDead = player_.health().dead();
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
  snapshot_.enemies.clear();
  for (const auto& enemy : enemies_.enemies()) {
    snapshot_.enemies.push_back({enemy.position, std::string(EnemyCatalog::all()[enemy.definitionIndex].id)});
  }
  snapshot_.pickups.clear();
  for (const auto& pickup : pickups_) snapshot_.pickups.push_back({pickup.position, pickup.type});
}

}  // namespace isaac::model
