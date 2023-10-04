#ifndef HAMURABI_DETAIL
#define HAMURABI_DETAIL

#include <string>

#include "Resources.hpp"

namespace hamurabi::detail {

constexpr Round kLastRound = 10;

constexpr Bushels kMinAcrePrice = 17;
constexpr Bushels kMaxAcrePrice = 26;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateAcrePrice(T &generator);

constexpr Bushels kMinGrainHarvestedFromAcre = 1;
constexpr Bushels kMaxGrainHarvestedFromAcre = 6;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateGrainHarvestedFromAcre(T &generator);

constexpr Bushels kMinGrainEatenByRatsFactor = 0;
constexpr Bushels kMaxGrainEatenByRatsFactor = 7;
constexpr Bushels kGrainEatenByRatsDivisor = 100;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateGrainEatenByRats(T &generator, Bushels grain_after_harvest);

constexpr Acres kAreaCanPlantWithBushel = 2;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Bushels GrainToPlantArea(Acres area) noexcept;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Acres AreaCanPlantWithGrain(Bushels grain) noexcept;

constexpr Acres kAreaToPlantPerPerson = 10;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Acres AreaCanPlantWithPopulation(People population) noexcept;

struct FeedPeopleResult final {
    Bushels grain_left;
    People dead;
};

constexpr auto kGrainPerPerson = 20;

[[nodiscard("result is used later to change game state")]]
constexpr static inline FeedPeopleResult FeedPeople(People population, Bushels grain_to_feed) noexcept;

constexpr People kMaxDeadFromHungerPercent = 100;
constexpr People kMinDeadFromHungerPercentToGameOver = 45;

[[nodiscard("it is important to track if the game is over")]]
constexpr static inline bool IsGameOver(People dead_from_hunger, People population) noexcept;

using AcresSigned = std::make_signed_t<Acres>;
using PeopleSigned = std::make_signed_t<People>;
using BushelsSigned = std::make_signed_t<Bushels>;

constexpr PeopleSigned kMinArrivedPeople = 0;
constexpr PeopleSigned kMaxArrivedPeople = 50;

[[nodiscard("result is used later to change game state")]]
constexpr static inline People CountArrivedPeople(People dead, Bushels harvested_from_acre, Bushels grain) noexcept;

constexpr std::uint_fast16_t kMinPlaguePercent = 0;
constexpr std::uint_fast16_t kMaxPlaguePercent = 100;
constexpr std::uint_fast16_t kMaxPlagueCanOccurPercent = 15;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline bool GenerateIsPlague(T &generator);

constexpr auto kInsertTagIndent = "    ";
constexpr auto kInsertTagDelim = ':';
constexpr auto kInsertGameTag = "hamurabi";
constexpr auto kInsertCurrentRoundTag = "current_round";
constexpr auto kInsertPopulationTag = "population";
constexpr auto kInsertAreaTag = "area";
constexpr auto kInsertGrainTag = "grain";
constexpr auto kInsertAcrePriceTag = "acre_price";
constexpr auto kInsertDeadFromHungerTag = "dead_from_hunger";
constexpr auto kInsertDeadFromHungerInTotalTag = "dead_from_hunger_in_total";
constexpr auto kInsertArrivedTag = "arrived";
constexpr auto kInsertGrainFromAcreTag = "grain_from_acre";
constexpr auto kInsertGrainEatenByRatsTag = "grain_eaten_by_rats";
constexpr auto kInsertIsPlagueTag = "is_plague";

static inline std::string &TrimLeft(std::string &string);

static inline std::string &TrimRight(std::string &string);

static inline std::string &Trim(std::string &string);

static inline std::string &ExtractUntilTagDelim(std::istream &istream, std::string &buffer);

static inline void ExtractCurrentRound(std::istream &istream, std::string &buffer, Round &current_round);

static inline void ExtractPopulation(std::istream &istream, std::string &buffer, People &population);

static inline void ExtractArea(std::istream &istream, std::string &buffer, Acres &area);

static inline void ExtractGrain(std::istream &istream, std::string &buffer, Bushels &grain);

static inline void ExtractAcrePrice(std::istream &istream, std::string &buffer, Bushels &acre_price);

static inline void ExtractDeadFromHunger(std::istream &istream, std::string &buffer, People &dead_from_hunger);

static inline void ExtractDeadFromHungerInTotal(std::istream &istream, std::string &buffer, People &dead_in_total);

static inline void ExtractArrived(std::istream &istream, std::string &buffer, People &arrived);

static inline void ExtractGrainFromAcre(std::istream &istream, std::string &buffer, Bushels &grain_from_acre);

static inline void ExtractGrainEatenByRats(std::istream &istream, std::string &buffer, Bushels &grain_eaten_by_rats);

static inline void ExtractIsPlague(std::istream &istream, std::string &buffer, bool &is_plague);

}

#include "Detail.inl"

#endif //HAMURABI_DETAIL
