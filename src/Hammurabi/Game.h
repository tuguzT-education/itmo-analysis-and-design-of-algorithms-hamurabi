#ifndef HAMMURABI_GAME_H_
#define HAMMURABI_GAME_H_

#include "Resources.h"

namespace hammurabi {

// TODO class Game

class Game {
  public:
    // TODO methods
  private:
    // TODO fields
};

template<class Generator>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateAcrePrice(Generator &generator);

constexpr Bushels kGrainPerPerson{20};
constexpr Acres kAreaToPlantPerPerson{10};
constexpr Acres kAreaCanPlantWithBushel{2};

template<class Generator>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainHarvestedFromAcre(Generator &generator);

template<class Generator>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainEatenByRats(Generator &generator, Bushels grain_after_harvest);

struct FeedPeopleResult {
    Bushels grain_left{};
    People dead{};
};

[[nodiscard("result is used later to change game state")]]
constexpr FeedPeopleResult FeedPeople(People population, Bushels grain_to_feed) noexcept;

[[nodiscard("it is important to track if the game is over")]]
constexpr bool IsGameOver(People dead, People population) noexcept;

[[nodiscard("result is used later to change game state")]]
constexpr People CountNewPeople(People dead, Bushels harvested_from_acre, Bushels grain) noexcept;

template<class Generator>
[[nodiscard("result of the next call could differ from the current result")]]
bool GenerateIsPlague(Generator &generator);

enum class Statistics : std::uint8_t {
    D = 1, C, B, A
};

[[nodiscard("result should be presented to the user")]]
constexpr Statistics CountStatistics(People average_dead_percent, Acres area_by_person) noexcept;

}

#endif //HAMMURABI_GAME_H_
