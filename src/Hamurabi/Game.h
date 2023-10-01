#ifndef HAMURABI_GAME_H_
#define HAMURABI_GAME_H_

#include <random>
#include <algorithm>

#include "Game_decl.h"

namespace hamurabi {

namespace detail {

constexpr Round kLastRound = 10;

constexpr Bushels kMinAcrePrice = 17;
constexpr Bushels kMaxAcrePrice = 26;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateAcrePrice(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinAcrePrice, kMaxAcrePrice};
    return distribution(generator);
}

constexpr Bushels kMinGrainHarvestedFromAcre = 1;
constexpr Bushels kMaxGrainHarvestedFromAcre = 6;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainHarvestedFromAcre(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainHarvestedFromAcre, kMaxGrainHarvestedFromAcre};
    return distribution(generator);
}

constexpr Bushels kMinGrainEatenByRatsFactor = 0;
constexpr Bushels kMaxGrainEatenByRatsFactor = 7;
constexpr Bushels kGrainEatenByRatsDivisor = 100;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainEatenByRats(T &generator, const Bushels grain_after_harvest) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainEatenByRatsFactor, kMaxGrainEatenByRatsFactor};
    const auto generated_value = distribution(generator);
    return (grain_after_harvest * generated_value) / kGrainEatenByRatsDivisor;
}

constexpr Acres kAreaCanPlantWithBushel = 2;

[[nodiscard("result is used later to change game state")]]
constexpr Bushels GrainToPlantArea(const Acres area) noexcept {
    return area / kAreaCanPlantWithBushel;
}

[[nodiscard("result is used later to change game state")]]
constexpr Acres AreaCanPlantWithGrain(const Bushels grain) noexcept {
    return grain * kAreaCanPlantWithBushel;
}

constexpr Acres kAreaToPlantPerPerson = 10;

[[nodiscard("result is used later to change game state")]]
constexpr Acres AreaCanPlantWithPopulation(const People population) noexcept {
    return population * kAreaToPlantPerPerson;
}

struct FeedPeopleResult final {
    Bushels grain_left{0};
    People dead{0};
};

constexpr auto kGrainPerPerson = 20;

[[nodiscard("result is used later to change game state")]]
constexpr FeedPeopleResult FeedPeople(const People population, const Bushels grain_to_feed) noexcept {
    const auto needed_grain = population * kGrainPerPerson;
    if (needed_grain > grain_to_feed) {
        const auto dead = (needed_grain - grain_to_feed - 1) / kGrainPerPerson + 1;
        return {.grain_left = 0, .dead = dead};
    }
    const auto grain_left = grain_to_feed - needed_grain;
    return {.grain_left = grain_left, .dead = 0};
}

constexpr People kMaxDeadFromHungerPercent = 100;
constexpr People kMinDeadFromHungerPercentToGameOver = 45;

[[nodiscard("it is important to track if the game is over")]]
constexpr bool IsGameOver(const People dead_from_hunger, const People population) noexcept {
    const auto percentage = (dead_from_hunger * kMaxDeadFromHungerPercent) / population;
    return percentage > kMinDeadFromHungerPercentToGameOver;
}

using PeopleSigned = std::make_signed_t<People>;
using BushelsSigned = std::make_signed_t<Bushels>;

constexpr PeopleSigned kMinArrivedPeople = 0;
constexpr PeopleSigned kMaxArrivedPeople = 50;

[[nodiscard("result is used later to change game state")]]
constexpr People CountArrivedPeople(const People dead,
                                    const Bushels harvested_from_acre,
                                    const Bushels grain) noexcept {
    const auto dead_signed = static_cast<PeopleSigned>(dead);
    const auto harvested_from_acre_signed = static_cast<BushelsSigned>(harvested_from_acre);
    const auto grain_signed = static_cast<BushelsSigned>(grain);
    const auto calculation = (dead_signed / 2) + ((5 - harvested_from_acre_signed) * grain_signed / 600) + 1;
    const auto clamped = std::clamp(calculation, kMinArrivedPeople, kMaxArrivedPeople);
    return static_cast<People>(clamped);
}

constexpr std::uint_fast16_t kMinPlaguePercent = 0;
constexpr std::uint_fast16_t kMaxPlaguePercent = 100;
constexpr std::uint_fast16_t kMaxPlagueCanOccurPercent = 15;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
bool GenerateIsPlague(T &generator) {
    static std::uniform_int_distribution distribution{kMinPlaguePercent, kMaxPlaguePercent};
    return distribution(generator) <= kMaxPlagueCanOccurPercent;
}

}

constexpr NotEnoughArea::NotEnoughArea(const Acres area) noexcept
    : area_{area} {}

constexpr Acres NotEnoughArea::Area() const noexcept {
    return area_;
}

constexpr NotEnoughGrain::NotEnoughGrain(const Bushels grain) noexcept
    : grain_{grain} {}

constexpr Bushels NotEnoughGrain::Grain() const noexcept {
    return grain_;
}

constexpr NotEnoughPeople::NotEnoughPeople(const People population) noexcept
    : population_{population} {}

constexpr People NotEnoughPeople::Population() const noexcept {
    return population_;
}

constexpr GameOver::GameOver(const People dead_from_hunger) noexcept
    : dead_from_hunger_{dead_from_hunger} {}

constexpr People GameOver::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr GameStatistics::GameStatistics(const Acres area,
                                         const People population,
                                         const People dead_from_hunger) noexcept
    : average_dead_from_hunger_percent_{dead_from_hunger / detail::kLastRound},
      dead_from_hunger_{dead_from_hunger},
      area_by_person_{area / population} {}

constexpr People GameStatistics::AverageDeadFromHungerPercent() const noexcept {
    return average_dead_from_hunger_percent_;
}

constexpr People GameStatistics::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr Acres GameStatistics::AreaByPerson() const noexcept {
    return area_by_person_;
}

constexpr GameStatistics::Rank GameStatistics::CalculateRank() const noexcept {
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

template<class T>
constexpr AreaToBuyResult AreaToBuy::New(const Acres area_to_buy, const Game<T> &game) noexcept {
    const auto grain = game.Grain();
    const auto acre_price = game.AcrePrice();
    const auto total_price = area_to_buy * acre_price;
    if (total_price > grain) {
        return NotEnoughGrain{grain};
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
        return NotEnoughArea{area};
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
        return NotEnoughGrain{grain};
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
        return NotEnoughArea{area};
    }
    const auto grain = game.Grain();
    if (area_to_plant > detail::AreaCanPlantWithGrain(grain)) {
        return NotEnoughGrain{grain};
    }
    const auto population = game.Population();
    if (area_to_plant > detail::AreaCanPlantWithPopulation(population)) {
        return NotEnoughPeople{population};
    }
    return AreaToPlant{area_to_plant};
}

constexpr AreaToPlant::operator Acres() const noexcept {
    return area_to_plant_;
}

constexpr AreaToPlant::AreaToPlant(const Acres area_to_plant) noexcept
    : area_to_plant_{area_to_plant} {}

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
        return GameOver{dead_from_hunger_};
    }
    if (current_round_ > detail::kLastRound) {
        return GameEnd{};
    }
    current_round_ += 1;

    const auto area_to_buy = static_cast<Acres>(input.area_to_buy);
    area_ += area_to_buy;
    const Bushels grain_to_buy_area = area_to_buy * acre_price_;
    grain_ -= grain_to_buy_area;

    const auto area_to_sell = static_cast<Acres>(input.area_to_sell);
    area_ -= area_to_sell;
    const Bushels grain_to_sell_area = area_to_sell * acre_price_;
    grain_ += grain_to_sell_area;

    const auto area_to_plant = static_cast<Acres>(input.area_to_plant);
    grain_from_acre_ = detail::GenerateGrainHarvestedFromAcre(generator_);
    const Bushels grain_harvested = area_to_plant * grain_from_acre_;
    grain_ += grain_harvested;
    const Bushels grain_to_plant_area = detail::GrainToPlantArea(area_to_plant);
    grain_ -= grain_to_plant_area;

    const auto grain_to_feed = static_cast<Bushels>(input.grain_to_feed);
    const auto feed_people_result = detail::FeedPeople(population_, grain_to_feed);
    grain_ += feed_people_result.grain_left;
    grain_ -= grain_to_feed;
    const auto old_population = population_;
    dead_from_hunger_ = feed_people_result.dead;
    population_ -= dead_from_hunger_;
    dead_from_hunger_in_total_ += dead_from_hunger_;
    if (detail::IsGameOver(dead_from_hunger_, old_population)) {
        is_game_over_ = true;
        return GameOver{dead_from_hunger_};
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
        return GameStatistics{area_, population_, dead_from_hunger_in_total_};
    }
    return std::nullopt;
}

}

#endif //HAMURABI_GAME_H_
