#include "model/Components.h"

namespace isaac::model {

HealthComponent::HealthComponent(int containers, int red, int shields)
    : containers_(std::max(1, containers)), red_(std::max(0, red)), shields_(std::max(0, shields)) {
  enforceCap();
}

void HealthComponent::enforceCap() {
  containers_ = std::clamp(containers_, 1, MaxCombinedHearts);
  red_ = std::clamp(red_, 0, containers_);
  shields_ = std::clamp(shields_, 0, MaxCombinedHearts - red_);
}

void HealthComponent::addContainer(int amount) { containers_ += amount; enforceCap(); }
void HealthComponent::healRed(int amount) { red_ += amount; enforceCap(); }
void HealthComponent::addShield(int amount) { shields_ += amount; enforceCap(); }

void HealthComponent::damage(int amount) {
  amount = std::max(0, amount);
  const int absorbed = std::min(shields_, amount);
  shields_ -= absorbed;
  red_ = std::max(0, red_ - (amount - absorbed));
}

bool Inventory::spendCoins(int amount) {
  if (amount < 0 || coins_ < amount) return false;
  coins_ -= amount;
  return true;
}

bool Inventory::useBomb() {
  if (bombs_ <= 0) return false;
  --bombs_;
  return true;
}

bool Inventory::useKey() {
  if (keys_ <= 0) return false;
  --keys_;
  return true;
}

bool ShootComponent::consumeShot() {
  if (cooldown_ > 0.F) return false;
  cooldown_ = interval_;
  return true;
}

}  // namespace isaac::model

