#ifndef HAMURABI_STATISTICS_INL
#define HAMURABI_STATISTICS_INL

namespace hamurabi {

template<class T>
constexpr Statistics::Statistics(const Game<T> &game) noexcept
    : average_dead_from_hunger_percent_{game.DeadFromHungerInTotal() / detail::kLastRound},
      dead_from_hunger_{game.DeadFromHungerInTotal()},
      area_by_person_{game.Area() / game.Population()} {}

constexpr People Statistics::AverageDeadFromHungerPercent() const noexcept {
    return average_dead_from_hunger_percent_;
}

constexpr People Statistics::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr Acres Statistics::AreaByPerson() const noexcept {
    return area_by_person_;
}

constexpr Rank Statistics::Rank() const noexcept {
    const auto average_dead_percent = AverageDeadFromHungerPercent();
    const auto area_by_person = AreaByPerson();
    if (average_dead_percent > 33 && area_by_person < 7) {
        return Rank::D;
    }
    if (average_dead_percent > 10 && area_by_person < 9) {
        return Rank::C;
    }
    if (average_dead_percent > 3 && area_by_person < 10) {
        return Rank::B;
    }
    return Rank::A;
}

}

#endif //HAMURABI_STATISTICS_INL
