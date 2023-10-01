#ifndef HAMURABI_AREA_TO_BUY_INL
#define HAMURABI_AREA_TO_BUY_INL

namespace hamurabi {

template<class T>
constexpr AreaToBuyResult AreaToBuy::New(const Acres area_to_buy, const Game<T> &game) noexcept {
    const auto grain = game.Grain();
    const auto acre_price = game.AcrePrice();
    const auto total_price = area_to_buy * acre_price;
    if (total_price > grain) {
        return NotEnoughGrain{game};
    }
    return AreaToBuy{area_to_buy};
}

constexpr AreaToBuy::operator Acres() const noexcept {
    return area_to_buy_;
}

constexpr AreaToBuy::AreaToBuy(const Acres area_to_buy) noexcept
    : area_to_buy_{area_to_buy} {}

}

#endif //HAMURABI_AREA_TO_BUY_INL
