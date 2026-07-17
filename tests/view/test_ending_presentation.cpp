#include "view/EndingPresentation.h"

#include <cstdlib>

int main() {
  const auto defeat = isaac::view::endingPresentation(isaac::common::ScreenState::Defeat);
  const auto victory = isaac::view::endingPresentation(isaac::common::ScreenState::Victory);
  if (!defeat.usesDefeatTestament || defeat.title != "DEAR DIARY") return EXIT_FAILURE;
  if (victory.usesDefeatTestament || victory.title != "VICTORY" ||
      victory.subtitle != "ALL BOSSES DEFEATED") {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
