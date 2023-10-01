#ifndef HAMURABI_NOT_ENOUGH_PEOPLE_INL
#define HAMURABI_NOT_ENOUGH_PEOPLE_INL

namespace hamurabi {

template<class T>
constexpr NotEnoughPeople::NotEnoughPeople(const Game<T> &game) noexcept
    : population_{game.Population()} {}

constexpr People NotEnoughPeople::Population() const noexcept {
    return population_;
}

}

#endif //HAMURABI_NOT_ENOUGH_PEOPLE_INL
