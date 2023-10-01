#ifndef HAMURABI_NOT_ENOUGH_AREA_INL
#define HAMURABI_NOT_ENOUGH_AREA_INL

namespace hamurabi {

template<class T>
constexpr NotEnoughArea::NotEnoughArea(const Game<T> &game) noexcept
    : area_{game.Area()} {}

constexpr Acres NotEnoughArea::Area() const noexcept {
    return area_;
}

}

#endif //HAMURABI_NOT_ENOUGH_AREA_INL
