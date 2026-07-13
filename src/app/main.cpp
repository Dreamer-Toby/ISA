#include "app/Application.h"

#include <string_view>

int main(int argc, char** argv) {
  isaac::app::Application application;
  if (argc == 2 && std::string_view(argv[1]) == "--evidence-character-select") {
    application.prepareCharacterSelectEvidence();
  }
  return application.run();
}
