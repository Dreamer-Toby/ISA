#pragma once

#include "model/GameSession.h"
#include "resource/ResourceManager.h"
#include "view/GameView.h"
#include "viewmodel/GameViewModel.h"

namespace isaac::app {

class Application {
 public:
  Application();
  void prepareCharacterSelectEvidence();
  int run();

 private:
  model::GameSession session_;
  viewmodel::GameViewModel viewModel_;
  resource::ResourceManager resources_;
  view::GameView view_;
  bool evidenceMode_{};
};

}  // namespace isaac::app
