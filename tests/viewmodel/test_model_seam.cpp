#include "model/GameSessionInterface.h"
#include "viewmodel/GameViewModel.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr float Step = 1.F / 60.F;
int failures{};

void check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    ++failures;
  }
}

class FakeGameSession final : public isaac::model::GameSessionInterface {
 public:
  void selectCharacter(std::size_t index) override { selectedCharacter = index; }

  void update(float seconds, const isaac::model::GameplayInput& input) override {
    ++updateCount;
    lastSeconds = seconds;
    lastInput = input;
  }

  [[nodiscard]] const isaac::model::SessionSnapshot& snapshot() const override {
    return state;
  }

  [[nodiscard]] std::vector<isaac::model::ModelEvent> drainEvents() override {
    auto drained = events;
    events.clear();
    return drained;
  }

  [[nodiscard]] std::size_t selectableCharacterCount() const override { return 1; }

  [[nodiscard]] isaac::model::CharacterSummary selectableCharacter(std::size_t) const override {
    return character;
  }

  isaac::model::CharacterSummary character{"isaac", "Isaac", 3, 120.F, 3.5F, 0.F};
  isaac::model::SessionSnapshot state;
  isaac::model::GameplayInput lastInput;
  std::vector<isaac::model::ModelEvent> events;
  std::size_t selectedCharacter{};
  int updateCount{};
  float lastSeconds{};
};

}  // namespace

int main() {
  using isaac::common::ScreenState;
  using isaac::presentation::UserAction;

  FakeGameSession model;
  isaac::viewmodel::GameViewModel viewModel(model);
  int propertyChanges{};
  const auto connection = viewModel.displayProperty().changed().connect(
      [&](const isaac::presentation::DisplayState&) { ++propertyChanges; });

  const auto act = [&](UserAction action) {
    viewModel.actionCommand().execute(action);
    viewModel.tickCommand().execute(Step);
  };
  act(UserAction::Confirm);
  check(viewModel.displayProperty().get().screen == ScreenState::MainMenu,
        "command publishes the display property through the Model seam");
  check(propertyChanges == 1, "display property notifies its subscriber");

  act(UserAction::Confirm);
  act(UserAction::Confirm);
  check(model.updateCount == 1 && model.selectedCharacter == 0,
        "commands reach the fake Model adapter");

  viewModel.inputCommand().execute({{1.F, 0.F}, {0.F, -1.F}});
  viewModel.tickCommand().execute(Step);
  check(model.updateCount == 2 && model.lastSeconds == Step &&
            model.lastInput.movement.x == 1.F && model.lastInput.shooting.y == -1.F,
        "realtime input crosses the Model seam");

  int firstSubscriber{};
  int secondSubscriber{};
  const auto firstConnection = viewModel.presentationSignal().connect(
      [&](isaac::presentation::PresentationEvent) { ++firstSubscriber; });
  const auto secondConnection = viewModel.presentationSignal().connect(
      [&](isaac::presentation::PresentationEvent) { ++secondSubscriber; });
  model.events = {isaac::model::ModelEvent::Shot, isaac::model::ModelEvent::Hurt,
                  isaac::model::ModelEvent::Pickup};
  viewModel.tickCommand().execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 3,
        "Model events become multi-subscriber presentation signals");
  viewModel.tickCommand().execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 3,
        "drained Model events are not emitted twice");

  viewModel.presentationSignal().disconnect(firstConnection);
  model.events = {isaac::model::ModelEvent::Shot};
  viewModel.tickCommand().execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 4,
        "disconnecting one signal subscriber leaves the other active");

  viewModel.displayProperty().changed().disconnect(connection);
  viewModel.presentationSignal().disconnect(secondConnection);

  std::vector<std::string> fatalDamageOrder;
  const auto fatalPropertyConnection = viewModel.displayProperty().changed().connect(
      [&](const isaac::presentation::DisplayState& display) {
        if (display.screen == ScreenState::Defeat) fatalDamageOrder.emplace_back("property");
      });
  const auto fatalSignalConnection = viewModel.presentationSignal().connect(
      [&](isaac::presentation::PresentationEvent event) {
        if (event == isaac::presentation::PresentationEvent::Hurt) fatalDamageOrder.emplace_back("hurt");
        if (event == isaac::presentation::PresentationEvent::Defeat) fatalDamageOrder.emplace_back("defeat");
      });
  model.state.playerDead = true;
  model.events = {isaac::model::ModelEvent::Hurt};
  viewModel.tickCommand().execute(Step);
  check(fatalDamageOrder == std::vector<std::string>{"property", "hurt", "defeat"},
        "fatal damage publishes Property, Hurt, then Defeat in order");
  viewModel.displayProperty().changed().disconnect(fatalPropertyConnection);
  viewModel.presentationSignal().disconnect(fatalSignalConnection);

  FakeGameSession cainModel;
  cainModel.character = {"cain", "Cain", 2, 145.F, 4.2F, 1.F};
  cainModel.state.characterId = "cain";
  isaac::viewmodel::GameViewModel cainViewModel(cainModel);
  const auto& cainDisplay = cainViewModel.displayProperty().get();
  check(cainDisplay.selectionStyle == isaac::presentation::CharacterStyle::Cain &&
            cainDisplay.entities.front().characterStyle == isaac::presentation::CharacterStyle::Cain,
        "known Model character IDs become typed presentation styles");

  FakeGameSession unknownModel;
  unknownModel.character.id = "unknown";
  unknownModel.state.characterId = "unknown";
  isaac::viewmodel::GameViewModel unknownViewModel(unknownModel);
  const auto& unknownDisplay = unknownViewModel.displayProperty().get();
  check(unknownDisplay.selectionStyle == isaac::presentation::CharacterStyle::Isaac &&
            unknownDisplay.entities.front().characterStyle == isaac::presentation::CharacterStyle::Isaac,
        "unknown character IDs use the Isaac presentation fallback");

  FakeGameSession presentationModel;
  presentationModel.state.floor = 1;
  presentationModel.state.roomCleared = true;
  presentationModel.state.rooms = {
      {0, isaac::common::RoomType::Normal, 0, 0, true, true, true, true},
      {1, isaac::common::RoomType::Treasure, -1, 0, false, true, false, true},
      {2, isaac::common::RoomType::Secret, 0, -1, false, false, false, true},
  };
  presentationModel.state.doors = {
      {isaac::common::Direction::Left, isaac::common::RoomType::Treasure, true, false, false},
      {isaac::common::Direction::Up, isaac::common::RoomType::Secret, false, true, false},
  };
  presentationModel.state.obstacles = {
      {isaac::common::ObstacleType::Rock, {315.F, 270.F}, 28.F},
      {isaac::common::ObstacleType::Trap, {710.F, 430.F}, 20.F},
  };
  presentationModel.state.treasureItems = {{{480.F, 300.F}, "wiggle_worm"}};
  isaac::viewmodel::GameViewModel presentationViewModel(presentationModel);
  const auto& presentationDisplay = presentationViewModel.displayProperty().get();
  check(presentationDisplay.doors.size() == 1 && presentationDisplay.doors.front().locked &&
            presentationDisplay.doors.front().targetType == isaac::common::RoomType::Treasure,
        "ViewModel hides secret entrances and exposes visible typed door presentation data");
  const auto rock = std::ranges::find_if(presentationDisplay.entities, [](const auto& entity) {
    return entity.kind == isaac::common::EntityKind::Rock;
  });
  const auto trap = std::ranges::find_if(presentationDisplay.entities, [](const auto& entity) {
    return entity.kind == isaac::common::EntityKind::Trap;
  });
  const auto treasure = std::ranges::find_if(presentationDisplay.entities, [](const auto& entity) {
    return entity.kind == isaac::common::EntityKind::TreasureItem;
  });
  check(rock != presentationDisplay.entities.end() && rock->radius == 28.F &&
            trap != presentationDisplay.entities.end() && trap->radius == 20.F,
        "modeled rocks and traps cross the ViewModel seam as typed entities");
  check(treasure != presentationDisplay.entities.end() && treasure->itemId == "wiggle_worm",
        "treasure item identity crosses the ViewModel seam without exposing Model objects");

  FakeGameSession combatModel;
  combatModel.state.roomCleared = false;
  combatModel.state.enemies = {{{300.F, 300.F}, "fly"}, {{500.F, 300.F}, "pooter"}};
  combatModel.state.doors = {
      {isaac::common::Direction::Right, isaac::common::RoomType::Boss, false, false, true},
  };
  isaac::viewmodel::GameViewModel combatViewModel(combatModel);
  const auto& combatDisplay = combatViewModel.displayProperty().get();
  check(combatDisplay.doors.front().sealed,
        "combat room exposes sealed doors without a mission-overlay presentation model");

  FakeGameSession bossRewardModel;
  bossRewardModel.state.floor = 2;
  bossRewardModel.state.roomType = isaac::common::RoomType::Boss;
  bossRewardModel.state.roomCleared = true;
  isaac::viewmodel::GameViewModel bossRewardViewModel(bossRewardModel);
  const auto& bossRewardDisplay = bossRewardViewModel.displayProperty().get();
  check(bossRewardDisplay.trapdoorVisible,
        "cleared Boss room exposes the trapdoor without the removed mission overlay");

  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
