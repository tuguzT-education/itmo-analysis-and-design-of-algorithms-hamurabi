#ifndef HAMURABI_DETAIL
#define HAMURABI_DETAIL

#include <string>

#include "Resources.hpp"
#include "Serialization.hpp"

namespace hamurabi::detail {

extern const Round kFirstRound;
extern const Round kLastRound;

extern const People kStartPopulation;
extern const Acres kStartArea;
extern const Bushels kStartGrain;
extern const People kStartDeadFromHunger;
extern const People kStartArrived;
extern const Bushels kStartGrainFromAcre;
extern const Bushels kStartGrainEatenByRats;
extern const bool kStartIsPlague;
extern const bool kStartIsGameOver;

extern const Bushels kMinAcrePrice;
extern const Bushels kMaxAcrePrice;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateAcrePrice(T &generator);

extern const Bushels kMinGrainHarvestedFromAcre;
extern const Bushels kMaxGrainHarvestedFromAcre;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateGrainHarvestedFromAcre(T &generator);

extern const Bushels kMinGrainEatenByRatsFactor;
extern const Bushels kMaxGrainEatenByRatsFactor;
extern const Bushels kGrainEatenByRatsDivisor;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline Bushels GenerateGrainEatenByRats(T &generator, Bushels grain_after_harvest);

extern const Acres kAreaCanPlantWithBushel;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Bushels GrainToPlantArea(Acres area) noexcept;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Acres AreaCanPlantWithGrain(Bushels grain) noexcept;

extern const Acres kAreaToPlantPerPerson;

[[nodiscard("result is used later to change game state")]]
constexpr static inline Acres AreaCanPlantWithPopulation(People population) noexcept;

struct FeedPeopleResult final {
    Bushels grain_left;
    People dead;
};

extern const Bushels kGrainPerPerson;

[[nodiscard("result is used later to change game state")]]
constexpr static inline FeedPeopleResult FeedPeople(People population, Bushels grain_to_feed) noexcept;

extern const People kMaxDeadFromHungerPercent;
extern const People kMinDeadFromHungerPercentToGameOver;

[[nodiscard("it is important to track if the game is over")]]
constexpr static inline bool IsGameOver(People dead_from_hunger, People population) noexcept;

using AcresSigned = std::make_signed_t<Acres>;
using PeopleSigned = std::make_signed_t<People>;
using BushelsSigned = std::make_signed_t<Bushels>;

extern const PeopleSigned kMinArrivedPeople;
extern const PeopleSigned kMaxArrivedPeople;

[[nodiscard("result is used later to change game state")]]
constexpr static inline People CountArrivedPeople(People dead, Bushels harvested_from_acre, Bushels grain) noexcept;

extern const std::uint_fast16_t kMinPlaguePercent;
extern const std::uint_fast16_t kMaxPlaguePercent;
extern const std::uint_fast16_t kMaxPlagueCanOccurPercent;

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
static inline bool GenerateIsPlague(T &generator);

static inline constexpr std::string_view TrimLeft(std::string_view string) noexcept;

static inline constexpr std::string_view TrimRight(std::string_view string) noexcept;

static inline constexpr std::string_view Trim(std::string_view string) noexcept;

extern const char kInsertTagDelim;

static inline std::string &ExtractUntilTagDelim(std::istream &istream, std::string &buffer);

namespace ser = hamurabi::serialization;

extern const string_literal kInsertGameTag;
extern const string_literal kInsertCurrentRoundTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractCurrentRound(std::istream &istream, std::string &buffer,
                                                     Round &current_round, ser::Format format);

extern const string_literal kInsertPopulationTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractPopulation(std::istream &istream, std::string &buffer,
                                                   People &population, ser::Format format);

extern const string_literal kInsertAreaTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractArea(std::istream &istream, std::string &buffer,
                                             Acres &area, ser::Format format);

extern const string_literal kInsertGrainTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractGrain(std::istream &istream, std::string &buffer,
                                              Bushels &grain, ser::Format format);

extern const string_literal kInsertAcrePriceTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractAcrePrice(std::istream &istream, std::string &buffer,
                                                  Bushels &acre_price, ser::Format format);

extern const string_literal kInsertDeadFromHungerTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractDeadFromHunger(std::istream &istream, std::string &buffer,
                                                       People &dead_from_hunger, ser::Format format);

extern const string_literal kInsertDeadFromHungerInTotalTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractDeadFromHungerInTotal(std::istream &istream, std::string &buffer,
                                                              People &dead_in_total, ser::Format format);

extern const string_literal kInsertArrivedTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractArrived(std::istream &istream, std::string &buffer,
                                                People &arrived, ser::Format format);

extern const string_literal kInsertGrainFromAcreTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractGrainFromAcre(std::istream &istream, std::string &buffer,
                                                      Bushels &grain_from_acre, ser::Format format);

extern const string_literal kInsertGrainEatenByRatsTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractGrainEatenByRats(std::istream &istream, std::string &buffer,
                                                         Bushels &grain_eaten_by_rats, ser::Format format);

extern const string_literal kInsertIsPlagueTag;

[[nodiscard]]
static inline ser::ExtractResult ExtractIsPlague(std::istream &istream, std::string &buffer,
                                                 bool &is_plague, ser::Format format);

extern const string_literal kInsertTagIndent;

}

#include "Detail.inl"

#endif //HAMURABI_DETAIL
