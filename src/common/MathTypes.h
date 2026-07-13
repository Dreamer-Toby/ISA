#pragma once

#include <cmath>

namespace isaac::common {

struct Vec2 {
  float x{};
  float y{};

  [[nodiscard]] float lengthSquared() const { return x * x + y * y; }
  [[nodiscard]] Vec2 normalized() const {
    const float length = std::sqrt(lengthSquared());
    return length > 0.0001F ? Vec2{x / length, y / length} : Vec2{};
  }

  Vec2& operator+=(Vec2 other) { x += other.x; y += other.y; return *this; }
};

inline Vec2 operator+(Vec2 left, Vec2 right) { return {left.x + right.x, left.y + right.y}; }
inline Vec2 operator-(Vec2 left, Vec2 right) { return {left.x - right.x, left.y - right.y}; }
inline Vec2 operator*(Vec2 value, float scalar) { return {value.x * scalar, value.y * scalar}; }

struct Rect {
  float left{};
  float top{};
  float width{};
  float height{};

  [[nodiscard]] bool contains(Vec2 point) const {
    return point.x >= left && point.x <= left + width &&
           point.y >= top && point.y <= top + height;
  }
};

}  // namespace isaac::common

