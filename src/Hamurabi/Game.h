#ifndef HAMURABI_GAME_H_
#define HAMURABI_GAME_H_

#include <variant>
#include <random>
#include <algorithm>

namespace hamurabi {

using Round = std::uint_fast32_t;

using People = std::uint_fast32_t;
using Acres = std::uint_fast32_t;
using Bushels = std::uint_fast32_t;

template<class T>
class Game;

class NotEnoughArea {
  public:
    constexpr explicit NotEnoughArea(Acres area) noexcept;

    [[nodiscard]]
    constexpr Acres Area() const noexcept;

  private:
    Acres area_;
};

class NotEnoughGrain {
  public:
    constexpr explicit NotEnoughGrain(Bushels grain) noexcept;

    [[nodiscard]]
    constexpr Bushels Grain() const noexcept;

  private:
    Bushels grain_;
};

class NotEnoughPeople {
  public:
    constexpr explicit NotEnoughPeople(People people) noexcept;

    [[nodiscard]]
    constexpr People People() const noexcept;

  private:
    hamurabi::People people_;
};

class AreaToBuy;
using AreaToBuyResult = std::variant<AreaToBuy, NotEnoughGrain>;

class AreaToBuy {
  public:
    template<class T>
    constexpr static AreaToBuyResult New(Acres area_to_buy, const Game<T> &game) noexcept;

    constexpr explicit operator Acres() const noexcept;

  private:
    constexpr explicit AreaToBuy(Acres area_to_buy) noexcept;

    Acres area_to_buy_;
};

class AreaToSell;
using AreaToSellResult = std::variant<AreaToSell, NotEnoughArea>;

class AreaToSell {
  public:
    template<class T>
    constexpr static AreaToSellResult New(Acres area_to_sell, const Game<T> &game) noexcept;

    constexpr explicit operator Acres() const noexcept;

  private:
    constexpr explicit AreaToSell(Acres area_to_sell) noexcept;

    Acres area_to_sell_;
};

class GrainToFeed;
using GrainToFeedResult = std::variant<GrainToFeed, NotEnoughGrain>;

class GrainToFeed {
  public:
    template<class T>
    constexpr static GrainToFeedResult New(Bushels grain_to_feed, const Game<T> &game) noexcept;

    constexpr explicit operator Bushels() const noexcept;

  private:
    constexpr explicit GrainToFeed(Bushels grain_to_feed) noexcept;

    Bushels grain_to_feed_;
};

class AreaToPlant;
using AreaToPlantResult = std::variant<AreaToPlant, NotEnoughArea, NotEnoughGrain, NotEnoughPeople>;

class AreaToPlant {
  public:
    template<class T>
    constexpr static AreaToPlantResult New(Acres area_to_plant, const Game<T> &game) noexcept;

    constexpr explicit operator Acres() const noexcept;

  private:
    constexpr explicit AreaToPlant(Acres area_to_plant) noexcept;

    Acres area_to_plant_;
};

struct RoundInput {
    AreaToBuy area_to_buy;
    AreaToSell area_to_sell;
    GrainToFeed grain_to_feed;
    AreaToPlant area_to_plant;
};

struct RoundOutput {
    People dead_people_count{};
    People new_people_count{};
    Bushels grain_from_acre{};
    Bushels grain_eaten_by_rats{};
    bool is_plague{};
};

class GameOver {
  public:
    constexpr explicit GameOver(People dead) noexcept;

    [[nodiscard]]
    constexpr People Dead() const noexcept;

  private:
    People dead_;
};

struct EndStatistics {
    const People average_dead_percent;
    const People dead_citizens;
    const Acres area_by_person;

    enum class Rank : std::uint8_t {
        D = 2, C, B, A,
    };

    constexpr explicit EndStatistics(People average_dead_percent,
                                     People dead_citizens,
                                     Acres area_by_person) noexcept;

    [[nodiscard]]
    constexpr Rank CalculateRank() const noexcept;
};

using RoundResult = std::variant<RoundOutput, GameOver, EndStatistics>;

template<class T>
class Game final {
  public:
    Game(Game &&other) = default;
    Game &operator=(Game &&other) = default;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    explicit Game(T generator);

    [[nodiscard]]
    Round CurrentRound() const noexcept;

    [[nodiscard]]
    People Population() const noexcept;

    [[nodiscard]]
    Acres Area() const noexcept;

    [[nodiscard]]
    Bushels Grain() const noexcept;

    [[nodiscard]]
    Bushels AcrePrice() const noexcept;

    [[nodiscard("result should be presented to the user")]]
    RoundResult PlayRound(RoundInput input);

  private:
    Round current_round_;
    People population_;
    Acres area_;
    Bushels grain_;
    Bushels acre_price_;
    People dead_;
    T generator_;
};

namespace detail {

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateAcrePrice(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{17, 26};
    return distribution(generator);
}

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainHarvestedFromAcre(T &generator) {
    static std::uniform_int_distribution<Bushels> distribution{1, 6};
    return distribution(generator);
}

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
Bushels GenerateGrainEatenByRats(T &generator, const Bushels grain_after_harvest) {
    static std::uniform_int_distribution<Bushels> distribution{0, 7};
    const auto generated_value = distribution(generator);
    return (grain_after_harvest * generated_value) / 100;
}

struct FeedPeopleResult {
    Bushels grain_left{};
    People dead{};
};

[[nodiscard("result is used later to change game state")]]
constexpr FeedPeopleResult FeedPeople(const People population, const Bushels grain_to_feed) noexcept {
    constexpr auto kGrainPerPerson = 20;

    const auto needed_grain = population * kGrainPerPerson;
    if (needed_grain > grain_to_feed) {
        const auto dead = (needed_grain - grain_to_feed - 1) / kGrainPerPerson + 1;
        return {.grain_left = 0, .dead = dead};
    }
    const auto grain_left = grain_to_feed - needed_grain;
    return {.grain_left = grain_left, .dead = 0};
}

[[nodiscard("it is important to track if the game is over")]]
constexpr bool IsGameOver(const People dead, const People population) noexcept {
    const auto percentage = (dead * 100) / population;
    return percentage > 45;
}

[[nodiscard("result is used later to change game state")]]
constexpr People CountNewPeople(const People dead, const Bushels harvested_from_acre, const Bushels grain) noexcept {
    const auto dead_s = static_cast<std::int_fast32_t>(dead);
    const auto harvested_from_acre_s = static_cast<std::int_fast32_t>(harvested_from_acre);
    const auto grain_s = static_cast<std::int_fast32_t>(grain);
    const auto calculation = (dead_s / 2) + ((5 - harvested_from_acre_s) * grain_s / 600) + 1;

    const decltype(calculation) kMinNewPeople = 0;
    const decltype(calculation) kMaxNewPeople = 50;
    return std::clamp(calculation, kMinNewPeople, kMaxNewPeople);
}

template<class T>
[[nodiscard("result of the next call could differ from the current result")]]
bool GenerateIsPlague(T &generator) {
    static std::uniform_int_distribution distribution{0, 100};
    return distribution(generator) <= 15;
}

}

constexpr NotEnoughArea::NotEnoughArea(Acres area) noexcept: area_{area} {}

constexpr Acres NotEnoughArea::Area() const noexcept {
    return area_;
}

constexpr NotEnoughGrain::NotEnoughGrain(Bushels grain) noexcept: grain_{grain} {}

constexpr Bushels NotEnoughGrain::Grain() const noexcept {
    return grain_;
}

constexpr NotEnoughPeople::NotEnoughPeople(hamurabi::People people) noexcept: people_{people} {}

constexpr People NotEnoughPeople::People() const noexcept {
    return people_;
}

constexpr GameOver::GameOver(People dead) noexcept: dead_{dead} {}

constexpr People GameOver::Dead() const noexcept {
    return dead_;
}

constexpr EndStatistics::EndStatistics(const hamurabi::People average_dead_percent,
                                       const People dead_citizens,
                                       const hamurabi::Acres area_by_person) noexcept
    : average_dead_percent{average_dead_percent},
      dead_citizens{dead_citizens},
      area_by_person{area_by_person} {}

constexpr EndStatistics::Rank EndStatistics::CalculateRank() const noexcept {
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
constexpr AreaToBuyResult AreaToBuy::New(Acres area_to_buy, const Game<T> &game) noexcept {
    const auto total_price = area_to_buy * game.AcrePrice();
    if (total_price > game.Grain()) {
        return NotEnoughGrain{game.Grain()};
    }
    return AreaToBuy{area_to_buy};
}

constexpr AreaToBuy::operator Acres() const noexcept {
    return area_to_buy_;
}

constexpr AreaToBuy::AreaToBuy(Acres area_to_buy) noexcept: area_to_buy_{area_to_buy} {}

template<class T>
constexpr AreaToSellResult AreaToSell::New(Acres area_to_sell, const Game<T> &game) noexcept {
    if (area_to_sell > game.Area()) {
        return NotEnoughArea{game.Area()};
    }
    return AreaToSell{area_to_sell};
}

constexpr AreaToSell::operator Acres() const noexcept {
    return area_to_sell_;
}

constexpr AreaToSell::AreaToSell(Acres area_to_sell) noexcept: area_to_sell_{area_to_sell} {}

template<class T>
constexpr GrainToFeedResult GrainToFeed::New(Bushels grain_to_feed, const Game<T> &game) noexcept {
    if (grain_to_feed > game.Grain()) {
        return NotEnoughGrain{game.Grain()};
    }
    return GrainToFeed{grain_to_feed};
}

constexpr GrainToFeed::operator Bushels() const noexcept {
    return grain_to_feed_;
}

constexpr GrainToFeed::GrainToFeed(Bushels grain_to_feed) noexcept: grain_to_feed_{grain_to_feed} {}

template<class T>
constexpr AreaToPlantResult AreaToPlant::New(Acres area_to_plant, const Game<T> &game) noexcept {
    constexpr Acres kAreaCanPlantWithBushel = 2;
    constexpr Acres kAreaToPlantPerPerson = 10;

    if (area_to_plant > game.Area()) {
        return NotEnoughArea{game.Area()};
    }
    if (area_to_plant > game.Grain() * kAreaCanPlantWithBushel) {
        return NotEnoughGrain{game.Grain()};
    }
    if (area_to_plant > game.Population() * kAreaToPlantPerPerson) {
        return NotEnoughPeople{game.Population()};
    }
    return AreaToPlant{area_to_plant};
}

constexpr AreaToPlant::operator Acres() const noexcept {
    return area_to_plant_;
}

constexpr AreaToPlant::AreaToPlant(Acres area_to_plant) noexcept: area_to_plant_{area_to_plant} {}

template<class T>
Game<T>::Game(T generator): generator_{generator},
                            current_round_{1},
                            population_{100},
                            area_{1000},
                            grain_{2800},
                            dead_{0} {
    acre_price_ = detail::GenerateAcrePrice(generator_);
}

template<class T>
Round Game<T>::CurrentRound() const noexcept {
    return current_round_;
}

template<class T>
People Game<T>::Population() const noexcept {
    return population_;
}

template<class T>
Acres Game<T>::Area() const noexcept {
    return area_;
}

template<class T>
Bushels Game<T>::Grain() const noexcept {
    return grain_;
}

template<class T>
Bushels Game<T>::AcrePrice() const noexcept {
    return acre_price_;
}

template<class T>
RoundResult Game<T>::PlayRound(const RoundInput input) {
    // TODO return result
    return GameOver{population_};
}

}

#endif //HAMURABI_GAME_H_
