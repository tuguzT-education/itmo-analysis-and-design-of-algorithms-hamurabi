#ifndef HAMURABI_GAME_H_
#define HAMURABI_GAME_H_

#include "Game_decl.h"
#include "Detail.h"

namespace hamurabi {

template<class T>
constexpr NotEnoughArea::NotEnoughArea(const Game<T> &game) noexcept
    : area_{game.Area()} {}

constexpr Acres NotEnoughArea::Area() const noexcept {
    return area_;
}

template<class T>
constexpr NotEnoughGrain::NotEnoughGrain(const Game<T> &game) noexcept
    : grain_{game.Grain()} {}

constexpr Bushels NotEnoughGrain::Grain() const noexcept {
    return grain_;
}

template<class T>
constexpr NotEnoughPeople::NotEnoughPeople(const Game<T> &game) noexcept
    : population_{game.Population()} {}

constexpr People NotEnoughPeople::Population() const noexcept {
    return population_;
}

template<class T>
constexpr GameOver::GameOver(const Game<T> &game) noexcept
    : dead_from_hunger_{game.DeadFromHunger()} {}

constexpr People GameOver::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

template<class T>
constexpr GameStatistics::GameStatistics(const Game<T> &game) noexcept
    : average_dead_from_hunger_percent_{game.DeadFromHungerInTotal() / detail::kLastRound},
      dead_from_hunger_{game.DeadFromHungerInTotal()},
      area_by_person_{game.Area() / game.Population()} {}

constexpr People GameStatistics::AverageDeadFromHungerPercent() const noexcept {
    return average_dead_from_hunger_percent_;
}

constexpr People GameStatistics::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr Acres GameStatistics::AreaByPerson() const noexcept {
    return area_by_person_;
}

constexpr GameRank GameStatistics::Rank() const noexcept {
    const auto average_dead_percent = AverageDeadFromHungerPercent();
    const auto area_by_person = AreaByPerson();
    if (average_dead_percent > 33 && area_by_person < 7) {
        return GameRank::D;
    }
    if (average_dead_percent > 10 && area_by_person < 9) {
        return GameRank::C;
    }
    if (average_dead_percent > 3 && area_by_person < 10) {
        return GameRank::B;
    }
    return GameRank::A;
}

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

template<class T>
constexpr AreaToSellResult AreaToSell::New(const Acres area_to_sell, const Game<T> &game) noexcept {
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

template<class T>
constexpr GrainToFeedResult GrainToFeed::New(const Bushels grain_to_feed, const Game<T> &game) noexcept {
    const auto grain = game.Grain();
    if (grain_to_feed > grain) {
        return NotEnoughGrain{game};
    }
    return GrainToFeed{grain_to_feed};
}

constexpr GrainToFeed::operator Bushels() const noexcept {
    return grain_to_feed_;
}

constexpr GrainToFeed::GrainToFeed(const Bushels grain_to_feed) noexcept
    : grain_to_feed_{grain_to_feed} {}

template<class T>
constexpr AreaToPlantResult AreaToPlant::New(const Acres area_to_plant, const Game<T> &game) noexcept {
    const auto area = game.Area();
    if (area_to_plant > area) {
        return NotEnoughArea{game};
    }
    const auto grain = game.Grain();
    if (area_to_plant > detail::AreaCanPlantWithGrain(grain)) {
        return NotEnoughGrain{game};
    }
    const auto population = game.Population();
    if (area_to_plant > detail::AreaCanPlantWithPopulation(population)) {
        return NotEnoughPeople{game};
    }
    return AreaToPlant{area_to_plant};
}

constexpr AreaToPlant::operator Acres() const noexcept {
    return area_to_plant_;
}

constexpr AreaToPlant::AreaToPlant(const Acres area_to_plant) noexcept
    : area_to_plant_{area_to_plant} {}

template<class T>
constexpr RoundInputResult RoundInput::New(const hamurabi::AreaToBuy area_to_buy,
                                           const hamurabi::AreaToSell area_to_sell,
                                           const hamurabi::GrainToFeed grain_to_feed,
                                           const hamurabi::AreaToPlant area_to_plant,
                                           const Game<T> &game) noexcept {
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

template<class T>
Game<T>::Game(T generator)
    : generator_{generator},
      current_round_{1},
      population_{100},
      area_{1000},
      grain_{2800},
      dead_from_hunger_{0},
      dead_from_hunger_in_total_{0},
      arrived_{5},
      grain_from_acre_{3},
      grain_eaten_by_rats_{200},
      is_plague_{false},
      is_game_over_{false} {
    acre_price_ = detail::GenerateAcrePrice(generator_);
}

template<class T>
constexpr Round Game<T>::CurrentRound() const noexcept {
    return current_round_;
}

template<class T>
constexpr People Game<T>::Population() const noexcept {
    return population_;
}

template<class T>
constexpr Acres Game<T>::Area() const noexcept {
    return area_;
}

template<class T>
constexpr Bushels Game<T>::Grain() const noexcept {
    return grain_;
}

template<class T>
constexpr Bushels Game<T>::AcrePrice() const noexcept {
    return acre_price_;
}

template<class T>
constexpr People Game<T>::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

template<class T>
constexpr People Game<T>::DeadFromHungerInTotal() const noexcept {
    return dead_from_hunger_in_total_;
}

template<class T>
constexpr People Game<T>::Arrived() const noexcept {
    return arrived_;
}

template<class T>
constexpr Bushels Game<T>::GrainFromAcre() const noexcept {
    return grain_from_acre_;
}

template<class T>
constexpr Bushels Game<T>::GrainEatenByRats() const noexcept {
    return grain_eaten_by_rats_;
}

template<class T>
constexpr bool Game<T>::IsPlague() const noexcept {
    return is_plague_;
}

template<class T>
RoundResult Game<T>::PlayRound(const RoundInput input) {
    if (is_game_over_) {
        return GameOver{*this};
    }
    if (current_round_ > detail::kLastRound) {
        return GameEnd{};
    }
    current_round_ += 1;

    const auto area_to_buy = static_cast<Acres>(input.AreaToBuy());
    area_ += area_to_buy;
    const Bushels grain_to_buy_area = area_to_buy * acre_price_;
    grain_ -= grain_to_buy_area;

    const auto area_to_sell = static_cast<Acres>(input.AreaToSell());
    area_ -= area_to_sell;
    const Bushels grain_to_sell_area = area_to_sell * acre_price_;
    grain_ += grain_to_sell_area;

    const auto area_to_plant = static_cast<Acres>(input.AreaToPlant());
    grain_from_acre_ = detail::GenerateGrainHarvestedFromAcre(generator_);
    const Bushels grain_harvested = area_to_plant * grain_from_acre_;
    grain_ += grain_harvested;
    const Bushels grain_to_plant_area = detail::GrainToPlantArea(area_to_plant);
    grain_ -= grain_to_plant_area;

    const auto grain_to_feed = static_cast<Bushels>(input.GrainToFeed());
    const auto feed_people_result = detail::FeedPeople(population_, grain_to_feed);
    grain_ += feed_people_result.grain_left;
    grain_ -= grain_to_feed;
    const auto old_population = population_;
    dead_from_hunger_ = feed_people_result.dead;
    population_ -= dead_from_hunger_;
    dead_from_hunger_in_total_ += dead_from_hunger_;
    if (detail::IsGameOver(dead_from_hunger_, old_population)) {
        is_game_over_ = true;
        return GameOver{*this};
    }

    grain_eaten_by_rats_ = detail::GenerateGrainEatenByRats(generator_, grain_);
    grain_ -= grain_eaten_by_rats_;

    arrived_ = detail::CountArrivedPeople(dead_from_hunger_, grain_from_acre_, grain_);
    population_ += arrived_;

    is_plague_ = detail::GenerateIsPlague(generator_);
    if (is_plague_) {
        population_ /= 2;
    }

    acre_price_ = detail::GenerateAcrePrice(generator_);
    if (current_round_ > detail::kLastRound) {
        return GameEnd{};
    }
    return Continue{};
}

template<class T>
std::optional<GameStatistics> Game<T>::Statistics() const noexcept {
    if (current_round_ > detail::kLastRound) {
        return GameStatistics{*this};
    }
    return std::nullopt;
}

}

#endif //HAMURABI_GAME_H_
