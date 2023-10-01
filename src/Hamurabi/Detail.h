#ifndef HAMURABI_DETAIL_H_
#define HAMURABI_DETAIL_H_

#include <random>
#include <algorithm>

#include "Resources.h"

namespace hamurabi::detail {

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

using AcresSigned = std::make_signed_t<Acres>;
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

#endif //HAMURABI_DETAIL_H_
