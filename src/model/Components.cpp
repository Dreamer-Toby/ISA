#include "model/Components.h"

namespace isaac::model {

HealthComponent::HealthComponent(int containers, int red, int shields)
    : containers_(std::max(1, containers)), redHalfUnits_(std::max(0, red) * 2),
      shieldHalfUnits_(std::max(0, shields) * 2) {
  enforceCap();
}

void HealthComponent::enforceCap() {
  containers_ = std::clamp(containers_, 1, MaxCombinedHearts);
  redHalfUnits_ = std::clamp(redHalfUnits_, 0, containers_ * 2);
  shieldHalfUnits_ = std::clamp(shieldHalfUnits_, 0,
                                MaxCombinedHearts * 2 - redHalfUnits_);
}

void HealthComponent::addContainer(int amount) { containers_ += amount; enforceCap(); }
void HealthComponent::healRed(int amount) { redHalfUnits_ += amount * 2; enforceCap(); }
void HealthComponent::addShield(int amount) { shieldHalfUnits_ += amount * 2; enforceCap(); }

void HealthComponent::damage(int amount) { damageHalfUnits(amount * 2); }

void HealthComponent::damageHalfUnits(int amount) {
  amount = std::max(0, amount);
  const int absorbed = std::min(shieldHalfUnits_, amount);
  shieldHalfUnits_ -= absorbed;
  redHalfUnits_ = std::max(0, redHalfUnits_ - (amount - absorbed));
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
