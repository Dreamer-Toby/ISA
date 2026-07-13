#include "model/GameSession.h"

#include "model/CharacterCatalog.h"

#include <algorithm>

namespace isaac::model {

GameSession::GameSession() : player_(CharacterCatalog::at(0)) { rebuildSnapshot(); }

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

  if (input.shooting.lengthSquared() > 0.1F && player_.canShoot()) {
    const auto direction = input.shooting.normalized();
    projectiles_.push_back({player_.position(), direction * player_.shooting().projectileSpeed(),
                            player_.shooting().damage(), 1.6F, true, true});
    ++snapshot_.totalShots;
  }
  for (auto& projectile : projectiles_) projectile.update(seconds);
  std::erase_if(projectiles_, [](const Projectile& projectile) { return !projectile.alive; });
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
}

}  // namespace isaac::model
