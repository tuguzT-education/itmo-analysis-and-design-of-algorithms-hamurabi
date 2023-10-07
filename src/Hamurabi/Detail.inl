#ifndef HAMURABI_DETAIL_INL
#define HAMURABI_DETAIL_INL

#include <random>
#include <algorithm>

namespace hamurabi::detail {

constexpr Round kLastRound = 10;

constexpr Bushels kMinAcrePrice = 17;
constexpr Bushels kMaxAcrePrice = 26;

template<class T>
Bushels GenerateAcrePrice(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinAcrePrice, kMaxAcrePrice};
    return distribution(generator);
}

constexpr Bushels kMinGrainHarvestedFromAcre = 1;
constexpr Bushels kMaxGrainHarvestedFromAcre = 6;

template<class T>
Bushels GenerateGrainHarvestedFromAcre(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainHarvestedFromAcre, kMaxGrainHarvestedFromAcre};
    return distribution(generator);
}

constexpr Bushels kMinGrainEatenByRatsFactor = 0;
constexpr Bushels kMaxGrainEatenByRatsFactor = 7;
constexpr Bushels kGrainEatenByRatsDivisor = 100;

template<class T>
Bushels GenerateGrainEatenByRats(T &generator, const Bushels grain_after_harvest) {
    static std::uniform_int_distribution<Bushels> distribution{kMinGrainEatenByRatsFactor, kMaxGrainEatenByRatsFactor};
    const auto generated_value = distribution(generator);
    return (grain_after_harvest * generated_value) / kGrainEatenByRatsDivisor;
}

constexpr Acres kAreaCanPlantWithBushel = 2;

constexpr Bushels GrainToPlantArea(const Acres area) noexcept {
    return area / kAreaCanPlantWithBushel;
}

constexpr Acres AreaCanPlantWithGrain(const Bushels grain) noexcept {
    return grain * kAreaCanPlantWithBushel;
}

constexpr Acres kAreaToPlantPerPerson = 10;

constexpr Acres AreaCanPlantWithPopulation(const People population) noexcept {
    return population * kAreaToPlantPerPerson;
}

constexpr Bushels kGrainPerPerson = 20;

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

constexpr bool IsGameOver(const People dead_from_hunger, const People population) noexcept {
    const auto percentage = (dead_from_hunger * kMaxDeadFromHungerPercent) / population;
    return percentage > kMinDeadFromHungerPercentToGameOver;
}

constexpr PeopleSigned kMinArrivedPeople = 0;
constexpr PeopleSigned kMaxArrivedPeople = 50;

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
bool GenerateIsPlague(T &generator) {
    static std::uniform_int_distribution distribution{kMinPlaguePercent, kMaxPlaguePercent};
    return distribution(generator) <= kMaxPlagueCanOccurPercent;
}

static inline bool TrimPredicate(const char character) noexcept {
    return !std::isspace(character);
}

std::string_view TrimLeft(std::string_view string) {
    const auto first = string.begin();
    const auto last = std::find_if(string.begin(), string.end(), TrimPredicate);
    const auto count = static_cast<std::size_t>(last - first);
    string.remove_prefix(std::min(count, string.size()));
    return string;
}

std::string_view TrimRight(std::string_view string) {
    const auto first = std::find_if(string.rbegin(), string.rend(), TrimPredicate).base();
    const auto last = string.end();
    const auto count = static_cast<std::size_t>(last - first);
    string.remove_suffix(std::min(count, string.size()));
    return string;
}

std::string_view Trim(const std::string_view string) {
    return TrimLeft(TrimRight(string));
}

constexpr string_literal kInsertTagDelim = ":";

std::string &ExtractUntilTagDelim(std::istream &istream, std::string &buffer) {
    buffer.clear();
    std::getline(istream, buffer);
    buffer = buffer.substr(0, buffer.find(detail::kInsertTagDelim, 0));
    buffer = Trim(buffer);
    return buffer;
}

constexpr string_literal kInsertGameTag = "hamurabi";
constexpr string_literal kInsertCurrentRoundTag = "current_round";

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

constexpr string_literal kInsertPopulationTag = "population";

ser::ExtractResult ExtractPopulation(std::istream &istream, std::string &buffer,
                                     People &population, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertPopulationTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> population;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertAreaTag = "area";

ser::ExtractResult ExtractArea(std::istream &istream, std::string &buffer,
                               Acres &area, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertAreaTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> area;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertGrainTag = "grain";

ser::ExtractResult ExtractGrain(std::istream &istream, std::string &buffer,
                                Bushels &grain, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertAcrePriceTag = "acre_price";

ser::ExtractResult ExtractAcrePrice(std::istream &istream, std::string &buffer,
                                    Bushels &acre_price, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertAcrePriceTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> acre_price;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertDeadFromHungerTag = "dead_from_hunger";

ser::ExtractResult ExtractDeadFromHunger(std::istream &istream, std::string &buffer,
                                         People &dead_from_hunger, [[maybe_unused]] const ser::Format format) {
    const auto has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertDeadFromHungerTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> dead_from_hunger;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertDeadFromHungerInTotalTag = "dead_from_hunger_in_total";

ser::ExtractResult ExtractDeadFromHungerInTotal(std::istream &istream, std::string &buffer,
                                                People &dead_in_total, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertDeadFromHungerInTotalTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> dead_in_total;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertArrivedTag = "arrived";

ser::ExtractResult ExtractArrived(std::istream &istream, std::string &buffer,
                                  People &arrived, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertArrivedTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> arrived;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertGrainFromAcreTag = "grain_from_acre";

ser::ExtractResult ExtractGrainFromAcre(std::istream &istream, std::string &buffer,
                                        Bushels &grain_from_acre, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainFromAcreTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain_from_acre;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertGrainEatenByRatsTag = "grain_eaten_by_rats";

ser::ExtractResult ExtractGrainEatenByRats(std::istream &istream, std::string &buffer,
                                           Bushels &grain_eaten_by_rats, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertGrainEatenByRatsTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> grain_eaten_by_rats;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertIsPlagueTag = "is_plague";

ser::ExtractResult ExtractIsPlague(std::istream &istream, std::string &buffer,
                                   bool &is_plague, [[maybe_unused]] const ser::Format format) {
    bool has_tag = ExtractUntilTagDelim(istream, buffer) == detail::kInsertIsPlagueTag;
    if (!has_tag) {
        return ser::ExtractResult::Error;
    }
    istream >> is_plague;
    return ser::ExtractResult::Success;
}

constexpr string_literal kInsertTagIndent = "    ";

}

#endif //HAMURABI_DETAIL_INL
