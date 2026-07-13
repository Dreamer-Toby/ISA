#pragma once

#include "common/GameTypes.h"
#include "common/MathTypes.h"
#include "model/Components.h"

#include <optional>
#include <vector>

namespace isaac::model {

struct Room {
  int id{};
  common::RoomType type{common::RoomType::Normal};
  int gridX{};
  int gridY{};
  bool cleared{};
  bool visited{};
  bool revealed{true};
  bool locked{};
  std::vector<int> neighbors;
};

class Level {
 public:
  explicit Level(unsigned seed = 1U);
  [[nodiscard]] int floorNumber() const { return floorIndex_ + 1; }
  [[nodiscard]] int currentRoomId() const { return currentRoom_; }
  [[nodiscard]] const Room& currentRoom() const { return rooms_.at(currentRoom_); }
  [[nodiscard]] const std::vector<Room>& rooms() const { return rooms_; }
  [[nodiscard]] bool connected() const;
  [[nodiscard]] bool hasRequiredRoomTypes() const;
  [[nodiscard]] std::optional<int> neighbor(common::Direction direction) const;
  bool enter(int roomId, Inventory& inventory, bool useBomb);
  void markCurrentCleared();
  bool advanceFloor();

 private:
  void generateFloor();
  unsigned seed_{};
  int floorIndex_{};
  int currentRoom_{};
  std::vector<Room> rooms_;
};

}  // namespace isaac::model
