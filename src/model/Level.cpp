#include "model/Level.h"

#include <algorithm>
#include <queue>

namespace isaac::model {

Level::Level() { generateFloor(); }

void Level::generateFloor() {
  rooms_ = {
      {0, common::RoomType::Normal, 0, 0, true, true, true, false, {1, 2}},
      {1, common::RoomType::Normal, 1, 0, false, false, true, false, {0, 3}},
      {2, common::RoomType::Treasure, -1, 0, true, false, true, true, {0}},
      {3, common::RoomType::Boss, 2, 0, false, false, true, false, {1}},
  };
  currentRoom_ = 0;
}

bool Level::connected() const {
  std::vector<bool> seen(rooms_.size());
  std::queue<int> pending;
  pending.push(0);
  seen[0] = true;
  while (!pending.empty()) {
    const int id = pending.front(); pending.pop();
    for (int next : rooms_[id].neighbors) if (!seen[next]) { seen[next] = true; pending.push(next); }
  }
  return std::ranges::all_of(seen, [](bool value) { return value; });
}

bool Level::hasRequiredRoomTypes() const {
  const auto count = [this](common::RoomType type) {
    return std::ranges::count_if(rooms_, [type](const Room& room) { return room.type == type; });
  };
  return rooms_.size() == 4 && count(common::RoomType::Normal) == 2 &&
         count(common::RoomType::Treasure) == 1 && count(common::RoomType::Boss) == 1;
}

std::optional<int> Level::neighbor(common::Direction direction) const {
  const auto& from = currentRoom();
  int dx{}, dy{};
  if (direction == common::Direction::Left) dx = -1;
  if (direction == common::Direction::Right) dx = 1;
  if (direction == common::Direction::Up) dy = -1;
  if (direction == common::Direction::Down) dy = 1;
  for (int id : from.neighbors) {
    const auto& candidate = rooms_[id];
    if (candidate.gridX == from.gridX + dx && candidate.gridY == from.gridY + dy) return id;
  }
  return std::nullopt;
}

bool Level::enter(int roomId, Inventory& inventory, bool useBomb) {
  if (!currentRoom().cleared || roomId < 0 || roomId >= static_cast<int>(rooms_.size())) return false;
  auto& target = rooms_[roomId];
  if (std::ranges::find(currentRoom().neighbors, roomId) == currentRoom().neighbors.end()) return false;
  if (!target.revealed) {
    if (!useBomb || !inventory.useBomb()) return false;
    target.revealed = true;
  }
  if (target.locked) {
    if (!inventory.useKey()) return false;
    target.locked = false;
  }
  currentRoom_ = roomId;
  target.visited = true;
  return true;
}

void Level::markCurrentCleared() { rooms_[currentRoom_].cleared = true; }

bool Level::advanceFloor() {
  if (currentRoom().type != common::RoomType::Boss || !currentRoom().cleared ||
      floorIndex_ >= MaxFloors - 1) return false;
  ++floorIndex_;
  generateFloor();
  return true;
}

}  // namespace isaac::model
