#include "model/GameSession.h"

#include <cmath>
#include <cstdlib>

int main() {
  isaac::model::GameSession session;
  session.update(1.F, {{1.F, 0.F}, {}});
  const auto snapshot = session.snapshot();
  return std::abs(snapshot.playerPosition.x - 660.F) < 0.01F ? EXIT_SUCCESS : EXIT_FAILURE;
}

