#ifndef HAMURABI_AREA_TO_SELL_INL
#define HAMURABI_AREA_TO_SELL_INL

namespace hamurabi {

template<class T>
constexpr AreaToSellResult AreaToSell::New(const Acres area_to_sell, const Game <T> &game) noexcept {
    const auto area = game.Area();
    if (area_to_sell > area) {
        return NotEnoughArea{game};
    }
    return AreaToSell{area_to_sell};
}

constexpr AreaToSell::operator Acres() const noexcept {
    return area_to_sell_;
}

constexpr AreaToSell::AreaToSell(const Acres area_to_sell) noexcept
    : area_to_sell_{area_to_sell} {}

}

#endif //HAMURABI_AREA_TO_SELL_INL
