#pragma once

#include "model/GameSession.h"
#include "resource/ResourceManager.h"
#include "view/GameView.h"
#include "viewmodel/GameViewModel.h"

namespace isaac::app {

class Application {
 public:
  Application();
  int run();

 private:
  model::GameSession session_;
  viewmodel::GameViewModel viewModel_;
  resource::ResourceManager resources_;
  view::GameView view_;
};

}  // namespace isaac::app

