#ifndef HAMURABI_DETAIL_INL
#define HAMURABI_DETAIL_INL

#include <random>
#include <algorithm>

namespace hamurabi::detail {

template<class T>
Bushels GenerateAcrePrice(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinAcrePrice, kMaxAcrePrice};
    return distribution(generator);
}

template<class T>
Bushels GenerateGrainHarvestedFromAcre(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainHarvestedFromAcre, kMaxGrainHarvestedFromAcre};
    return distribution(generator);
}

template<class T>
Bushels GenerateGrainEatenByRats(T &generator, const Bushels grain_after_harvest) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainEatenByRatsFactor, kMaxGrainEatenByRatsFactor};
    const auto generated_value = distribution(generator);
    return (grain_after_harvest * generated_value) / kGrainEatenByRatsDivisor;
}

constexpr Bushels GrainToPlantArea(const Acres area) noexcept {
    return area / kAreaCanPlantWithBushel;
}

constexpr Acres AreaCanPlantWithGrain(const Bushels grain) noexcept {
    return grain * kAreaCanPlantWithBushel;
}

constexpr Acres AreaCanPlantWithPopulation(const People population) noexcept {
    return population * kAreaToPlantPerPerson;
}

constexpr FeedPeopleResult FeedPeople(const People population, const Bushels grain_to_feed) noexcept {
    const auto needed_grain = population * kGrainPerPerson;
    if (needed_grain > grain_to_feed) {
        const auto dead = (needed_grain - grain_to_feed - 1) / kGrainPerPerson + 1;
        return {.grain_left = 0, .dead = dead};
    }
    const auto grain_left = grain_to_feed - needed_grain;
    return {.grain_left = grain_left, .dead = 0};
}

constexpr bool IsGameOver(const People dead_from_hunger, const People population) noexcept {
    const auto percentage = (dead_from_hunger * kMaxDeadFromHungerPercent) / population;
    return percentage > kMinDeadFromHungerPercentToGameOver;
}

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

template<class T>
bool GenerateIsPlague(T &generator) {
    static std::uniform_int_distribution distribution{kMinPlaguePercent, kMaxPlaguePercent};
    return distribution(generator) <= kMaxPlagueCanOccurPercent;
}

std::string &TrimLeft(std::string &string) {
    const auto predicate = [](const auto item) -> bool {
        return !std::isspace(item);
    };
    const auto first = string.begin();
    const auto last = std::find_if(string.begin(), string.end(), predicate);
    string.erase(first, last);
    return string;
}

std::string &TrimRight(std::string &string) {
    const auto predicate = [](const auto item) -> bool {
        return !std::isspace(item);
    };
    const auto first = std::find_if(string.rbegin(), string.rend(), predicate).base();
    const auto last = string.end();
    string.erase(first, last);
    return string;
}

std::string &Trim(std::string &string) {
    return TrimLeft(TrimRight(string));
}

std::string &ExtractUntilTagDelim(std::istream &istream, std::string &buffer) {
    buffer.clear();
    std::getline(istream, buffer);
    buffer = buffer.substr(0, buffer.find(detail::kInsertTagDelim, 0));
    return Trim(buffer);
}

ser::ExtractResult ExtractCurrentRound(std::istream &istream, std::string &buffer,
                                       Round &current_round, [[maybe_unused]] const ser::Format format) {
    const auto has_game_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGameTag;
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertCurrentRoundTag;
    if (!has_game_tag || !has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> current_round;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractPopulation(std::istream &istream, std::string &buffer,
                                     People &population, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertPopulationTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> population;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractArea(std::istream &istream, std::string &buffer,
                               Acres &area, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertAreaTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> area;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractGrain(std::istream &istream, std::string &buffer,
                                Bushels &grain, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractAcrePrice(std::istream &istream, std::string &buffer,
                                    Bushels &acre_price, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertAcrePriceTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> acre_price;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractDeadFromHunger(std::istream &istream, std::string &buffer,
                                         People &dead_from_hunger, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertDeadFromHungerTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> dead_from_hunger;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractDeadFromHungerInTotal(std::istream &istream, std::string &buffer,
                                                People &dead_in_total, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertDeadFromHungerInTotalTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> dead_in_total;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractArrived(std::istream &istream, std::string &buffer,
                                  People &arrived, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertArrivedTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> arrived;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractGrainFromAcre(std::istream &istream, std::string &buffer,
                                        Bushels &grain_from_acre, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainFromAcreTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain_from_acre;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractGrainEatenByRats(std::istream &istream, std::string &buffer,
                                           Bushels &grain_eaten_by_rats, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainEatenByRatsTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain_eaten_by_rats;
    return ser::ExtractResult::Success;
}

ser::ExtractResult ExtractIsPlague(std::istream &istream, std::string &buffer,
                                   bool &is_plague, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertIsPlagueTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> is_plague;
    return ser::ExtractResult::Success;
}

}

#endif //HAMURABI_DETAIL_INL
