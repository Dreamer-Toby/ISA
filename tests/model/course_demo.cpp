#include "model/GameSession.h"
#include "viewmodel/GameViewModel.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {
constexpr float Step = 1.F / 60.F;

void tick(isaac::viewmodel::GameViewModel& vm, isaac::presentation::RealtimeInput input = {}) {
  vm.inputCommand().execute(input);
  vm.tickCommand().execute(Step);
}

void pressConfirm(isaac::viewmodel::GameViewModel& vm) {
  vm.actionCommand().execute(isaac::presentation::UserAction::Confirm);
  tick(vm);
}

bool driveToRoom(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm,
                 int roomId, isaac::common::Vec2 movement, bool bomb = false) {
  for (int frame = 0; frame < 2000 && session.level().currentRoomId() != roomId; ++frame) {
    isaac::presentation::RealtimeInput input;
    input.movement = movement;
    if (std::abs(movement.x) > 0.1F) {
      const float y = session.snapshot().playerPosition.y;
      input.movement.y = y > 160.F ? -0.35F : (y < 140.F ? 0.35F : 0.F);
    }
    if (bomb) vm.actionCommand().execute(isaac::presentation::UserAction::UseBomb);
    tick(vm, input);
  }
  const bool reached = session.level().currentRoomId() == roomId;
  if (!reached) {
    const auto position = session.snapshot().playerPosition;
    std::cerr << "demo detail: room " << session.level().currentRoomId() << " -> " << roomId
              << " stalled at " << position.x << ',' << position.y
              << " cleared=" << session.snapshot().roomCleared
              << " dead=" << session.snapshot().playerDead << '\n';
  }
  return reached;
}

bool clearCurrentRoom(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm) {
  for (int frame = 0; frame < 20000 && !session.snapshot().roomCleared; ++frame) {
    isaac::presentation::RealtimeInput input;
    isaac::common::Vec2 target{};
    if (!session.snapshot().enemies.empty()) target = session.snapshot().enemies.front().position;
    else if (!session.snapshot().bosses.empty()) target = session.snapshot().bosses.front().position;
    const auto toward = (target - session.snapshot().playerPosition).normalized();
    input.shooting = toward.lengthSquared() > 0.1F ? toward : isaac::common::Vec2{1.F, 0.F};
    input.movement = {-toward.y, toward.x};
    vm.actionCommand().execute(isaac::presentation::UserAction::UseActive);
    tick(vm, input);
  }
  return session.snapshot().roomCleared && !session.snapshot().playerDead;
}

void collectPickups(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm) {
  for (int frame = 0; frame < 2000 && !session.snapshot().pickups.empty(); ++frame) {
    isaac::presentation::RealtimeInput input;
    input.movement = session.snapshot().pickups.front().position - session.snapshot().playerPosition;
    tick(vm, input);
  }
}

bool collectTreasure(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm) {
  for (int frame = 0; frame < 2000 && !session.snapshot().treasureItems.empty(); ++frame) {
    isaac::presentation::RealtimeInput input;
    input.movement = session.snapshot().treasureItems.front().position - session.snapshot().playerPosition;
    tick(vm, input);
  }
  return session.snapshot().roomRewardCollected && session.snapshot().treasureItems.empty();
}

bool completeFloor(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm) {
  if (!driveToRoom(session, vm, 2, {-1.F, 0.F})) { std::cerr << "demo detail: cannot enter treasure\n"; return false; }
  if (!collectTreasure(session, vm)) { std::cerr << "demo detail: treasure not collected\n"; return false; }
  if (!driveToRoom(session, vm, 0, {1.F, 0.F})) { std::cerr << "demo detail: cannot leave treasure\n"; return false; }
  if (!driveToRoom(session, vm, 1, {1.F, 0.F})) { std::cerr << "demo detail: cannot enter normal\n"; return false; }
  if (!clearCurrentRoom(session, vm)) {
    const auto& state = session.snapshot();
    std::cerr << "demo detail: normal not clear, dead=" << state.playerDead
              << " enemies=" << state.enemies.size()
              << " player=" << state.playerPosition.x << ',' << state.playerPosition.y
              << " sine=" << session.player().shooting().sineProjectiles() << '\n';
    for (const auto& enemy : state.enemies) {
      std::cerr << "demo detail: remaining " << enemy.id << " at "
                << enemy.position.x << ',' << enemy.position.y << '\n';
    }
    return false;
  }
  collectPickups(session, vm);
  if (!driveToRoom(session, vm, 3, {1.F, 0.F})) { std::cerr << "demo detail: cannot enter boss\n"; return false; }
  if (!clearCurrentRoom(session, vm)) { std::cerr << "demo detail: boss not clear, dead=" << session.snapshot().playerDead << '\n'; return false; }
  if (session.level().rooms().size() != 4) {
    std::cerr << "demo detail: boss clear added a forbidden fifth room\n";
    return false;
  }
  pressConfirm(vm);
  return true;
}
}  // namespace

int main() {
#define REQUIRE(condition, message) do { if (!(condition)) { std::cerr << "demo FAIL: " << message << '\n'; return EXIT_FAILURE; } } while (false)
  isaac::model::GameSession session(1U);
  isaac::viewmodel::GameViewModel vm(session);
  pressConfirm(vm);
  pressConfirm(vm);
  vm.actionCommand().execute(isaac::presentation::UserAction::NavigateRight);
  tick(vm);
  pressConfirm(vm);
  REQUIRE(session.snapshot().characterId == "magdalene", "non-default Magdalene selection");

  const int keysBeforeTreasure = session.player().inventory().keys();
  REQUIRE(completeFloor(session, vm), "complete floor 1 four-room route");
  REQUIRE(session.player().inventory().keys() == keysBeforeTreasure - 1,
          "floor 1 treasure consumes exactly one key");
  REQUIRE(session.level().floorNumber() == 2, "advance to floor 2");

  REQUIRE(completeFloor(session, vm) && session.level().floorNumber() == 2, "complete floor 2");
  REQUIRE(vm.displayProperty().get().screen == isaac::common::ScreenState::Victory, "victory screen");

  std::cout << "demo: menu->Magdalene->treasure->combat/Breakfast drop->Boss->2 floors->victory\n";
  return EXIT_SUCCESS;
#undef REQUIRE
}
