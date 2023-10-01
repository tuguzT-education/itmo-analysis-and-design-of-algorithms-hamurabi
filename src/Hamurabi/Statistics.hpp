#ifndef HAMURABI_STATISTICS
#define HAMURABI_STATISTICS

#include "Resources.hpp"
#include "Game.fwd"

namespace hamurabi {

enum class Rank : std::uint8_t {
    D = 2, C, B, A,
};

class Statistics final {
  public:
    template<class T>
    constexpr explicit Statistics(const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr People AverageDeadFromHungerPercent() const noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

    [[nodiscard]]
    constexpr Acres AreaByPerson() const noexcept;

    [[nodiscard]]
    constexpr Rank Rank() const noexcept;

  private:
    People average_dead_from_hunger_percent_;
    People dead_from_hunger_;
    Acres area_by_person_;
};

}

#include "Statistics.inl"

#endif //HAMURABI_STATISTICS
