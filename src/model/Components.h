#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace isaac::model {

class HealthComponent {
 public:
  static constexpr int MaxCombinedHearts = 12;

  HealthComponent(int containers = 3, int red = 3, int shields = 0);
  void addContainer(int amount);
  void healRed(int amount);
  void addShield(int amount);
  void damage(int amount);
  [[nodiscard]] bool dead() const { return red_ + shields_ <= 0; }
  [[nodiscard]] int containers() const { return containers_; }
  [[nodiscard]] int red() const { return red_; }
  [[nodiscard]] int shields() const { return shields_; }

 private:
  void enforceCap();
  int containers_{};
  int red_{};
  int shields_{};
};

class Inventory {
 public:
  bool spendCoins(int amount);
  bool useBomb();
  bool useKey();
  void addCoins(int amount) { coins_ = std::max(0, coins_ + amount); }
  void addBombs(int amount) { bombs_ = std::max(0, bombs_ + amount); }
  void addKeys(int amount) { keys_ = std::max(0, keys_ + amount); }
  [[nodiscard]] int coins() const { return coins_; }
  [[nodiscard]] int bombs() const { return bombs_; }
  [[nodiscard]] int keys() const { return keys_; }
  [[nodiscard]] const std::string& activeItem() const { return activeItem_; }
  [[nodiscard]] const std::vector<std::string>& passiveItems() const { return passiveItems_; }
  [[nodiscard]] const std::string& trinket() const { return trinket_; }
  void setActiveItem(std::string item) { activeItem_ = std::move(item); }
  void addPassiveItem(std::string item) { passiveItems_.push_back(std::move(item)); }
  void setTrinket(std::string item) { trinket_ = std::move(item); }

 private:
  int coins_{};
  int bombs_{1};
  int keys_{2};
  std::string activeItem_{"None"};
  std::vector<std::string> passiveItems_;
  std::string trinket_{"None"};
};

class ShootComponent {
 public:
  ShootComponent(float damage = 3.5F, float shotsPerSecond = 2.5F, float projectileSpeed = 430.F)
      : damage_(damage), interval_(1.F / shotsPerSecond), projectileSpeed_(projectileSpeed) {}
  void tick(float seconds) { cooldown_ = std::max(0.F, cooldown_ - seconds); }
  bool consumeShot();
  [[nodiscard]] float damage() const { return damage_; }
  [[nodiscard]] float shotsPerSecond() const { return 1.F / interval_; }
  [[nodiscard]] float projectileSpeed() const { return projectileSpeed_; }
  void addDamage(float amount) { damage_ = std::clamp(damage_ + amount, 1.F, 20.F); }
  void multiplyInterval(float factor) { interval_ = std::clamp(interval_ * factor, 0.12F, 2.F); }

 private:
  float damage_{};
  float interval_{};
  float projectileSpeed_{};
  float cooldown_{};
};

}  // namespace isaac::model
