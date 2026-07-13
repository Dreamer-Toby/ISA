#include "app/Application.h"

#include <filesystem>
#include <string_view>

int main(int argc, char** argv) {
  isaac::app::Application application;
  if (argc == 2 && std::string_view(argv[1]) == "--evidence-character-select") {
    application.prepareCharacterSelectEvidence();
  } else if (argc == 3) {
    const std::string_view mode(argv[1]);
    using EvidenceScreen = isaac::app::Application::EvidenceScreen;
    if (mode == "--capture-main-menu") {
      application.prepareEvidence(EvidenceScreen::MainMenu, std::filesystem::path(argv[2]));
    } else if (mode == "--capture-character-select") {
      application.prepareEvidence(EvidenceScreen::CharacterSelect, std::filesystem::path(argv[2]));
    } else if (mode == "--capture-gameplay") {
      application.prepareEvidence(EvidenceScreen::Gameplay, std::filesystem::path(argv[2]));
    } else if (mode == "--capture-pause") {
      application.prepareEvidence(EvidenceScreen::Paused, std::filesystem::path(argv[2]));
    }
  }
  return application.run();
}
