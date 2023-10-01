#ifndef HAMURABI_NOT_ENOUGH_GRAIN_INL
#define HAMURABI_NOT_ENOUGH_GRAIN_INL

namespace hamurabi {

template<class T>
constexpr NotEnoughGrain::NotEnoughGrain(const Game<T> &game) noexcept
    : grain_{game.Grain()} {}

constexpr Bushels NotEnoughGrain::Grain() const noexcept {
    return grain_;
}

}

#endif //HAMURABI_NOT_ENOUGH_GRAIN_INL
