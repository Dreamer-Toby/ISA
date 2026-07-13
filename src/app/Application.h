#pragma once

#include "model/GameSession.h"
#include "resource/ResourceManager.h"
#include "view/GameView.h"
#include "viewmodel/GameViewModel.h"

#include <filesystem>

namespace isaac::app {

class Application {
 public:
  enum class EvidenceScreen { MainMenu, CharacterSelect, Gameplay, Paused };

  Application();
  void prepareCharacterSelectEvidence();
  void prepareEvidence(EvidenceScreen screen, std::filesystem::path outputPath);
  int run();

 private:
  model::GameSession session_;
  viewmodel::GameViewModel viewModel_;
  resource::ResourceManager resources_;
  view::GameView view_;
  bool evidenceMode_{};
  std::filesystem::path evidencePath_;
  int evidenceFrames_{};
};

}  // namespace isaac::app
