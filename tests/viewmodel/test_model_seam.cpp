#include "model/GameSessionInterface.h"
#include "viewmodel/GameViewModel.h"

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
  using isaac::viewmodel::UserAction;

  FakeGameSession model;
  isaac::viewmodel::GameViewModel viewModel(model);
  int propertyChanges{};
  const auto connection = viewModel.properties().display.changed().connect(
      [&](const isaac::viewmodel::DisplayState&) { ++propertyChanges; });

  const auto act = [&](UserAction action) {
    viewModel.commands().action.execute(action);
    viewModel.commands().tick.execute(Step);
  };
  act(UserAction::Confirm);
  check(viewModel.properties().display.get().screen == ScreenState::MainMenu,
        "command publishes the display property through the Model seam");
  check(propertyChanges == 1, "display property notifies its subscriber");

  act(UserAction::Confirm);
  act(UserAction::Confirm);
  check(model.updateCount == 1 && model.selectedCharacter == 0,
        "commands reach the fake Model adapter");

  viewModel.commands().setInput.execute({{1.F, 0.F}, {0.F, -1.F}});
  viewModel.commands().tick.execute(Step);
  check(model.updateCount == 2 && model.lastSeconds == Step &&
            model.lastInput.movement.x == 1.F && model.lastInput.shooting.y == -1.F,
        "realtime input crosses the Model seam");

  int firstSubscriber{};
  int secondSubscriber{};
  const auto firstConnection = viewModel.signals().presentation.connect(
      [&](isaac::viewmodel::PresentationEvent) { ++firstSubscriber; });
  const auto secondConnection = viewModel.signals().presentation.connect(
      [&](isaac::viewmodel::PresentationEvent) { ++secondSubscriber; });
  model.events = {isaac::model::ModelEvent::Shot, isaac::model::ModelEvent::Hurt,
                  isaac::model::ModelEvent::Pickup};
  viewModel.commands().tick.execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 3,
        "Model events become multi-subscriber presentation signals");
  viewModel.commands().tick.execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 3,
        "drained Model events are not emitted twice");

  viewModel.signals().presentation.disconnect(firstConnection);
  model.events = {isaac::model::ModelEvent::Shot};
  viewModel.commands().tick.execute(Step);
  check(firstSubscriber == 3 && secondSubscriber == 4,
        "disconnecting one signal subscriber leaves the other active");

  viewModel.properties().display.changed().disconnect(connection);
  viewModel.signals().presentation.disconnect(secondConnection);

  std::vector<std::string> fatalDamageOrder;
  const auto fatalPropertyConnection = viewModel.properties().display.changed().connect(
      [&](const isaac::viewmodel::DisplayState& display) {
        if (display.screen == ScreenState::Defeat) fatalDamageOrder.emplace_back("property");
      });
  const auto fatalSignalConnection = viewModel.signals().presentation.connect(
      [&](isaac::viewmodel::PresentationEvent event) {
        if (event == isaac::viewmodel::PresentationEvent::Hurt) fatalDamageOrder.emplace_back("hurt");
        if (event == isaac::viewmodel::PresentationEvent::Defeat) fatalDamageOrder.emplace_back("defeat");
      });
  model.state.playerDead = true;
  model.events = {isaac::model::ModelEvent::Hurt};
  viewModel.commands().tick.execute(Step);
  check(fatalDamageOrder == std::vector<std::string>{"property", "hurt", "defeat"},
        "fatal damage publishes Property, Hurt, then Defeat in order");
  viewModel.properties().display.changed().disconnect(fatalPropertyConnection);
  viewModel.signals().presentation.disconnect(fatalSignalConnection);

  FakeGameSession cainModel;
  cainModel.character = {"cain", "Cain", 2, 145.F, 4.2F, 1.F};
  cainModel.state.characterId = "cain";
  isaac::viewmodel::GameViewModel cainViewModel(cainModel);
  const auto& cainDisplay = cainViewModel.properties().display.get();
  check(cainDisplay.selectionStyle == isaac::viewmodel::CharacterStyle::Cain &&
            cainDisplay.entities.front().characterStyle == isaac::viewmodel::CharacterStyle::Cain,
        "known Model character IDs become typed presentation styles");

  FakeGameSession unknownModel;
  unknownModel.character.id = "unknown";
  unknownModel.state.characterId = "unknown";
  isaac::viewmodel::GameViewModel unknownViewModel(unknownModel);
  const auto& unknownDisplay = unknownViewModel.properties().display.get();
  check(unknownDisplay.selectionStyle == isaac::viewmodel::CharacterStyle::Isaac &&
            unknownDisplay.entities.front().characterStyle == isaac::viewmodel::CharacterStyle::Isaac,
        "unknown character IDs use the Isaac presentation fallback");

  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
