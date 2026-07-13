#include "model/CharacterCatalog.h"

namespace isaac::model {

const std::array<CharacterDefinition, 4>& CharacterCatalog::all() {
  static constexpr std::array<CharacterDefinition, 4> definitions{{
      {"isaac", "Isaac", 3, 3, 0, 180.F, 3.5F, 2.5F, 0.F},
      {"magdalene", "Magdalene", 4, 4, 0, 150.F, 3.5F, 2.3F, 0.F},
      {"cain", "Cain", 2, 2, 0, 205.F, 4.0F, 2.5F, 1.F},
      {"judas", "Judas", 1, 1, 3, 185.F, 5.2F, 2.35F, 0.F},
  }};
  return definitions;
}

const CharacterDefinition& CharacterCatalog::at(std::size_t index) {
  return all().at(index % all().size());
}

}  // namespace isaac::model

