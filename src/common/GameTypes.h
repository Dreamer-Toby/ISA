#pragma once

#include <cstdint>

namespace isaac::common {

enum class ScreenState : std::uint8_t {
  Start,
  MainMenu,
  Rankings,
  CharacterSelect,
  Playing,
  Paused,
  Defeat,
  Victory
};
enum class EntityKind : std::uint8_t { Player, Enemy, Boss, PlayerProjectile, EnemyProjectile, Pickup };
enum class RoomType : std::uint8_t { Normal, Treasure, Shop, Secret, Boss, Devil };
enum class PickupType : std::uint8_t { Coin, Bomb, Key, RedHeart, Shield, Active, Passive, Trinket };
enum class Direction : std::uint8_t { Up, Right, Down, Left };

}  // namespace isaac::common
