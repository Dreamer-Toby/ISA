#include "model/BossSystem.h"

#include <algorithm>
#include <cmath>

namespace isaac::model {

const std::array<BossDefinition, 3>& BossCatalog::all() {
  static constexpr std::array<BossDefinition, 3> definitions{{
      {"monstro", "Monstro", 75.F, BossPattern::LeapVolley},
      {"duke_of_flies", "Duke of Flies", 65.F, BossPattern::OrbitBurst},
      {"larry_jr", "Larry Jr.", 70.F, BossPattern::AxisDash},
  }};
  return definitions;
}

void BossSystem::spawnForFloor(int floor) {
  bosses_.clear();
  if (floor == 1) bosses_.push_back({0, {650.F, 280.F}, BossCatalog::all()[0].health, 1, 0.F, 1.F});
  if (floor == 2) {
    bosses_.push_back({1, {620.F, 210.F}, BossCatalog::all()[1].health, 1, 0.F, 0.8F});
    bosses_.push_back({2, {620.F, 380.F}, BossCatalog::all()[2].health, 1, 0.F, 1.2F});
  }
}

bool BossSystem::update(float seconds, Player& player, std::vector<Projectile>& projectiles) {
  bool playerHurt{};
  for (auto& boss : bosses_) {
    const auto& definition = BossCatalog::all()[boss.definitionIndex];
    boss.phaseTimer += seconds;
    boss.attackTimer -= seconds;
    boss.phase = boss.health <= definition.health * 0.5F ? 2 : 1;
    const auto toward = (player.position() - boss.position).normalized();
    const float pace = boss.phase == 2 ? 1.45F : 1.F;
    if (definition.pattern == BossPattern::LeapVolley) boss.position += toward * (45.F * pace * seconds);
    if (definition.pattern == BossPattern::OrbitBurst) {
      boss.position += common::Vec2{std::cos(boss.phaseTimer), std::sin(boss.phaseTimer)} * (35.F * seconds);
    }
    if (definition.pattern == BossPattern::AxisDash && std::fmod(boss.phaseTimer, 2.F) < 0.45F) {
      boss.position += common::Vec2{toward.x, 0.F}.normalized() * (150.F * seconds);
    }
    boss.position.x = std::clamp(boss.position.x, 90.F, 870.F);
    boss.position.y = std::clamp(boss.position.y, 130.F, 450.F);
    if ((boss.position - player.position()).lengthSquared() < 42.F * 42.F) {
      playerHurt = player.damage(1) || playerHurt;
    }

    if (boss.attackTimer <= 0.F) {
      constexpr std::array<common::Vec2, 4> cardinal{{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
      if (definition.pattern == BossPattern::OrbitBurst || definition.pattern == BossPattern::LeapVolley) {
        for (const auto direction : cardinal) projectiles.push_back({boss.position, direction * 250.F, 1.F, 2.2F, false, true});
      }
      boss.attackTimer = 1.4F / pace;
    }
  }

  for (auto& projectile : projectiles) {
    if (!projectile.alive) continue;
    if (projectile.friendly) {
      for (auto& boss : bosses_) {
        if (boss.health > 0.F && (projectile.position - boss.position).lengthSquared() < 34.F * 34.F) {
          boss.health -= projectile.damage; projectile.alive = false; break;
        }
      }
    } else if ((projectile.position - player.position()).lengthSquared() < 22.F * 22.F) {
      playerHurt = player.damage(1) || playerHurt;
      projectile.alive = false;
    }
  }
  std::erase_if(bosses_, [](const Boss& boss) { return boss.health <= 0.F; });
  std::erase_if(projectiles, [](const Projectile& projectile) { return !projectile.alive; });
  return playerHurt;
}

}  // namespace isaac::model
