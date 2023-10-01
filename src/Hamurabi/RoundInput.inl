#ifndef HAMURABI_ROUND_INPUT_INL
#define HAMURABI_ROUND_INPUT_INL

namespace hamurabi {

template<class T>
constexpr RoundInputResult RoundInput::New(const hamurabi::AreaToBuy area_to_buy,
                                           const hamurabi::AreaToSell area_to_sell,
                                           const hamurabi::GrainToFeed grain_to_feed,
                                           const hamurabi::AreaToPlant area_to_plant,
                                           const Game <T> &game) noexcept {
    const auto area = static_cast<detail::AcresSigned>(game.Area());
    const auto grain = static_cast<detail::BushelsSigned>(game.Grain());
    const auto acre_price = static_cast<detail::BushelsSigned>(game.AcrePrice());

    const auto area_to_buy_raw = static_cast<detail::AcresSigned>(static_cast<Acres>(area_to_buy));
    const auto area_to_sell_raw = static_cast<detail::AcresSigned>(static_cast<Acres>(area_to_sell));
    const auto grain_to_feed_raw = static_cast<detail::BushelsSigned>(static_cast<Bushels>(grain_to_feed));
    const auto area_to_plant_raw = static_cast<detail::AcresSigned>(static_cast<Acres>(area_to_plant));

    const auto area_needed = area_to_sell_raw + area_to_plant_raw - area_to_buy_raw;
    if (area_needed > area) {
        return NotEnoughArea{game};
    }

    const auto grain_to_plant = static_cast<detail::BushelsSigned>(detail::GrainToPlantArea(area_to_plant_raw));
    const auto grain_needed = (area_to_buy_raw * acre_price) + grain_to_feed_raw +
        grain_to_plant - (area_to_sell_raw * acre_price);
    if (grain_needed > grain) {
        return NotEnoughGrain{game};
    }

    return RoundInput{area_to_buy, area_to_sell, grain_to_feed, area_to_plant};
}

constexpr AreaToBuy RoundInput::AreaToBuy() const {
    return area_to_buy_;
}

constexpr AreaToSell RoundInput::AreaToSell() const {
    return area_to_sell_;
}

constexpr GrainToFeed RoundInput::GrainToFeed() const {
    return grain_to_feed_;
}

constexpr AreaToPlant RoundInput::AreaToPlant() const {
    return area_to_plant_;
}

constexpr RoundInput::RoundInput(const hamurabi::AreaToBuy area_to_buy,
                                 const hamurabi::AreaToSell area_to_sell,
                                 const hamurabi::GrainToFeed grain_to_feed,
                                 const hamurabi::AreaToPlant area_to_plant) noexcept
    : area_to_buy_{area_to_buy},
      area_to_sell_{area_to_sell},
      grain_to_feed_{grain_to_feed},
      area_to_plant_{area_to_plant} {}

}

#endif //HAMURABI_ROUND_INPUT_INL
