#include "model/CharacterCatalog.h"
#include "model/Components.h"
#include "model/GameSession.h"
#include "model/Level.h"

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
