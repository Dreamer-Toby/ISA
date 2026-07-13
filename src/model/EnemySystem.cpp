#include "model/EnemySystem.h"

#include <algorithm>
#include <cmath>

namespace isaac::model {

const std::array<EnemyDefinition, 6>& EnemyCatalog::all() {
  static constexpr std::array<EnemyDefinition, 6> definitions{{
      {"fly", "Fly", 5.F, 72.F, 1, MoveStrategy::Chase, AttackStrategy::Contact, DropStrategy::Coin},
      {"pooter", "Pooter", 7.F, 55.F, 1, MoveStrategy::KeepDistance, AttackStrategy::AimedShot, DropStrategy::None},
      {"spider", "Spider", 6.F, 95.F, 1, MoveStrategy::Wander, AttackStrategy::Contact, DropStrategy::Bomb},
      {"hopper", "Hopper", 9.F, 82.F, 1, MoveStrategy::Dash, AttackStrategy::Contact, DropStrategy::None},
      {"gaper", "Gaper", 12.F, 62.F, 1, MoveStrategy::Chase, AttackStrategy::Contact, DropStrategy::Key},
      {"clotty", "Clotty", 14.F, 0.F, 1, MoveStrategy::Stationary, AttackStrategy::RadialShot, DropStrategy::Coin},
  }};
  return definitions;
}

void EnemySystem::spawnForNormalRoom(int roomId, int floor) {
  enemies_.clear();
  const int count = 2 + (roomId + floor) % 3;
  for (int i = 0; i < count; ++i) {
    const std::size_t definitionIndex = static_cast<std::size_t>((roomId * 2 + floor + i) % 6);
    const auto& definition = EnemyCatalog::all()[definitionIndex];
    enemies_.push_back({definitionIndex, {300.F + 140.F * static_cast<float>(i), 220.F + 65.F * static_cast<float>(i % 2)},
                        definition.health, 0.8F + 0.25F * static_cast<float>(i), 0.F});
  }
}

void EnemySystem::dropFor(const Enemy& enemy, std::vector<Pickup>& pickups) const {
  const auto drop = EnemyCatalog::all()[enemy.definitionIndex].drop;
  if (drop == DropStrategy::None) return;
  common::PickupType type = common::PickupType::Coin;
  if (drop == DropStrategy::Bomb) type = common::PickupType::Bomb;
  if (drop == DropStrategy::Key) type = common::PickupType::Key;
  pickups.push_back({type, enemy.position});
}

void EnemySystem::update(float seconds, Player& player, std::vector<Projectile>& projectiles,
                         std::vector<Pickup>& pickups) {
  for (auto& enemy : enemies_) {
    const auto& definition = EnemyCatalog::all()[enemy.definitionIndex];
    enemy.attackTimer -= seconds;
    enemy.moveTimer += seconds;
    const auto toward = (player.position() - enemy.position).normalized();
    common::Vec2 movement{};
    if (definition.movement == MoveStrategy::Chase) movement = toward;
    if (definition.movement == MoveStrategy::KeepDistance) {
      const float distance = (player.position() - enemy.position).lengthSquared();
      movement = distance < 280.F * 280.F ? toward * -1.F : toward;
    }
    if (definition.movement == MoveStrategy::Wander) movement = {std::cos(enemy.moveTimer * 2.3F), std::sin(enemy.moveTimer * 1.7F)};
    if (definition.movement == MoveStrategy::Dash && std::fmod(enemy.moveTimer, 1.6F) < 0.35F) movement = toward * 2.2F;
    enemy.position += movement.normalized() * (definition.speed * seconds);
    enemy.position.x = std::clamp(enemy.position.x, 75.F, 885.F);
    enemy.position.y = std::clamp(enemy.position.y, 115.F, 465.F);

    if ((player.position() - enemy.position).lengthSquared() < 30.F * 30.F) player.damage(definition.contactDamage);
    if (enemy.attackTimer <= 0.F && definition.attack != AttackStrategy::Contact) {
      if (definition.attack == AttackStrategy::AimedShot) {
        projectiles.push_back({enemy.position, toward * 260.F, 1.F, 2.2F, false, true});
      } else {
        constexpr std::array<common::Vec2, 4> directions{{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
        for (auto direction : directions) projectiles.push_back({enemy.position, direction * 220.F, 1.F, 2.2F, false, true});
      }
      enemy.attackTimer = 1.6F;
    }
  }

  for (auto& projectile : projectiles) {
    if (!projectile.alive) continue;
    if (projectile.friendly) {
      for (auto& enemy : enemies_) {
        if (enemy.health > 0.F && (projectile.position - enemy.position).lengthSquared() < 22.F * 22.F) {
          enemy.health -= projectile.damage;
          projectile.alive = false;
          break;
        }
      }
    } else if ((projectile.position - player.position()).lengthSquared() < 22.F * 22.F) {
      player.damage(1);
      projectile.alive = false;
    }
  }
  for (const auto& enemy : enemies_) if (enemy.health <= 0.F) dropFor(enemy, pickups);
  std::erase_if(enemies_, [](const Enemy& enemy) { return enemy.health <= 0.F; });
  std::erase_if(projectiles, [](const Projectile& projectile) { return !projectile.alive; });
}

}  // namespace isaac::model
