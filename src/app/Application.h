#pragma once

#include "model/GameSession.h"
#include "resource/ResourceManager.h"
#include "view/GameView.h"
#include "viewmodel/GameViewModel.h"

#include <cstddef>

namespace isaac::app {

class Application {
 public:
  Application();
  ~Application();
  int run();

 private:
  model::GameSession session_;
  viewmodel::GameViewModel viewModel_;
  resource::ResourceManager resources_;
  view::GameView view_;
  std::size_t displayConnection_{};
  std::size_t presentationConnection_{};
};

}  // namespace isaac::app
