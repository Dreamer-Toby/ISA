#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace isaac::common {

template <typename... Args>
class Signal {
 public:
  using Connection = std::size_t;
  using Slot = std::function<void(Args...)>;

  [[nodiscard]] Connection connect(Slot slot) const {
    const auto connection = nextConnection_++;
    slots_.emplace_back(connection, std::move(slot));
    return connection;
  }

  void disconnect(Connection connection) const {
    std::erase_if(slots_, [connection](const auto& entry) { return entry.first == connection; });
  }

  void emit(Args... args) {
    const auto currentSlots = slots_;
    for (const auto& entry : currentSlots) entry.second(args...);
  }

 private:
  mutable Connection nextConnection_{1};
  mutable std::vector<std::pair<Connection, Slot>> slots_;
};

template <typename T>
class Property {
 public:
  Property() = default;
  explicit Property(T value) : value_(std::move(value)) {}

  [[nodiscard]] const T& get() const { return value_; }
  [[nodiscard]] const Signal<const T&>& changed() const { return changed_; }

  void set(T value) {
    value_ = std::move(value);
    changed_.emit(value_);
  }

 private:
  T value_{};
  Signal<const T&> changed_;
};

template <typename... Args>
class Command {
 public:
  using Action = std::function<void(Args...)>;

  Command() = default;
  explicit Command(Action action) : action_(std::move(action)) {}

  void execute(Args... args) const {
    if (!action_) return;
    action_(std::forward<Args>(args)...);
  }

 private:
  Action action_;
};

}  // namespace isaac::common
