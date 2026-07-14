#include "model/GameSession.h"
#include "viewmodel/GameViewModel.h"

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
  for (int frame = 0; frame < 500 && session.level().currentRoomId() != roomId; ++frame) {
    isaac::presentation::RealtimeInput input;
    input.movement = movement;
    if (bomb) vm.actionCommand().execute(isaac::presentation::UserAction::UseBomb);
    tick(vm, input);
  }
  return session.level().currentRoomId() == roomId;
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

bool completeCombatAndBoss(isaac::model::GameSession& session, isaac::viewmodel::GameViewModel& vm) {
  if (!driveToRoom(session, vm, 1, {1.F, 0.F})) { std::cerr << "demo detail: cannot enter normal\n"; return false; }
  if (!clearCurrentRoom(session, vm)) { std::cerr << "demo detail: normal not clear, dead=" << session.snapshot().playerDead << '\n'; return false; }
  collectPickups(session, vm);
  if (!driveToRoom(session, vm, 5, {1.F, 0.F})) { std::cerr << "demo detail: cannot enter boss\n"; return false; }
  if (!clearCurrentRoom(session, vm)) { std::cerr << "demo detail: boss not clear, dead=" << session.snapshot().playerDead << '\n'; return false; }
  pressConfirm(vm);
  return true;
}
}  // namespace

int main() {
#define REQUIRE(condition, message) do { if (!(condition)) { std::cerr << "demo FAIL: " << message << '\n'; return EXIT_FAILURE; } } while (false)
  isaac::model::GameSession session;
  isaac::viewmodel::GameViewModel vm(session);
  pressConfirm(vm);
  pressConfirm(vm);
  vm.actionCommand().execute(isaac::presentation::UserAction::NavigateRight);
  tick(vm);
  pressConfirm(vm);
  REQUIRE(session.snapshot().characterId == "magdalene", "non-default Magdalene selection");

  REQUIRE(driveToRoom(session, vm, 1, {1.F, 0.F}) && clearCurrentRoom(session, vm), "floor 1 normal combat");
  collectPickups(session, vm);
  REQUIRE(driveToRoom(session, vm, 0, {-1.F, 0.F}), "return to start");

  REQUIRE(driveToRoom(session, vm, 3, {0.F, 1.F}), "enter shop");
  pressConfirm(vm);
  REQUIRE(!session.player().inventory().passiveItems().empty(), "buy shop passive");
  REQUIRE(driveToRoom(session, vm, 0, {0.F, -1.F}), "leave shop");

  REQUIRE(driveToRoom(session, vm, 2, {-1.F, 0.F}), "enter treasure");
  pressConfirm(vm);
  REQUIRE(session.player().inventory().keys() == 0, "open chest with remaining key");
  REQUIRE(driveToRoom(session, vm, 0, {1.F, 0.F}), "leave treasure");

  REQUIRE(driveToRoom(session, vm, 4, {0.F, -1.F}, true), "bomb into secret");
  REQUIRE(session.player().inventory().trinket() != "None", "take secret trinket");
  REQUIRE(driveToRoom(session, vm, 0, {0.F, 1.F}), "leave secret");

  REQUIRE(driveToRoom(session, vm, 1, {1.F, 0.F}) && driveToRoom(session, vm, 5, {1.F, 0.F}), "enter floor 1 boss");
  REQUIRE(clearCurrentRoom(session, vm), "clear floor 1 boss");
  pressConfirm(vm);
  REQUIRE(session.level().floorNumber() == 2, "advance to floor 2");

  REQUIRE(completeCombatAndBoss(session, vm) && session.level().floorNumber() == 3, "complete floor 2");
  REQUIRE(completeCombatAndBoss(session, vm), "complete floor 3");
  REQUIRE(vm.displayProperty().get().screen == isaac::common::ScreenState::Victory, "victory screen");

  std::cout << "demo: menu->Magdalene->combat/drop->shop->chest->secret->3 floors->victory\n";
  return EXIT_SUCCESS;
#undef REQUIRE
}
