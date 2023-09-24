#include "Game.h"

#include <random>
#include <algorithm>

namespace hammurabi {

template<class Generator>
Bushels GenerateAcrePrice(Generator &generator) {
    static std::uniform_int_distribution<Bushels> distribution{17, 26};
    return distribution(generator);
}

template<class Generator>
Bushels GenerateGrainHarvestedFromAcre(Generator &generator) {
    static std::uniform_int_distribution<Bushels> distribution{1, 6};
    return distribution(generator);
}

template<class Generator>
Bushels GenerateGrainEatenByRats(Generator &generator, const Bushels grain_after_harvest) {
    static std::uniform_int_distribution<Bushels> distribution{0, 7};
    const auto generated_value = distribution(generator);
    return (grain_after_harvest * generated_value) / 100;
}

constexpr FeedPeopleResult FeedPeople(const People population, const Bushels grain_to_feed) noexcept {
    const auto needed_grain = population * 20;
    if (needed_grain > grain_to_feed) {
        const auto dead = (needed_grain - grain_to_feed - 1) / 20 + 1;
        return {.grain_left = 0, .dead = dead};
    }
    const auto grain_left = grain_to_feed - needed_grain;
    return {.grain_left = grain_left, .dead = 0};
}

constexpr bool IsGameOver(const People dead, const People population) noexcept {
    const auto percentage = (dead * 100) / population;
    return percentage > 45;
}

constexpr People CountNewPeople(const People dead, const Bushels harvested_from_acre, const Bushels grain) noexcept {
    const auto calculation = (dead / 2) + ((5 - static_cast<std::int_fast64_t>(harvested_from_acre)) * grain / 600) + 1;
    const decltype(calculation) kMinNewPeople = 0, kMaxNewPeople = 50;
    return std::clamp(calculation, kMinNewPeople, kMaxNewPeople);
}

template<class Generator>
bool GenerateIsPlague(Generator &generator) {
    static std::uniform_int_distribution distribution{0, 100};
    return distribution(generator) <= 15;
}

constexpr Statistics CountStatistics(People average_dead_percent, Acres area_by_person) noexcept {
    if (average_dead_percent > 33 && area_by_person < 7) {
        return Statistics::D;
    }
    if (average_dead_percent > 10 && area_by_person < 9) {
        return Statistics::C;
    }
    if (average_dead_percent > 3 && area_by_person < 10) {
        return Statistics::B;
    }
    return Statistics::A;
}

}
