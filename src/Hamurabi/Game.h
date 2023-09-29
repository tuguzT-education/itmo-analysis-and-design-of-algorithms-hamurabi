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

class NotEnoughArea final {
  public:
    constexpr explicit NotEnoughArea(Acres area) noexcept;

    [[nodiscard]]
    constexpr Acres Area() const noexcept;

  private:
    Acres area_;
};

class NotEnoughGrain final {
  public:
    constexpr explicit NotEnoughGrain(Bushels grain) noexcept;

    [[nodiscard]]
    constexpr Bushels Grain() const noexcept;

  private:
    Bushels grain_;
};

class NotEnoughPeople final {
  public:
    constexpr explicit NotEnoughPeople(People population) noexcept;

    [[nodiscard]]
    constexpr People Population() const noexcept;

  private:
    People population_;
};

class AreaToBuy;
using AreaToBuyResult = std::variant<AreaToBuy, NotEnoughGrain>;

class AreaToBuy final {
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

class AreaToSell final {
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

class GrainToFeed final {
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

class AreaToPlant final {
  public:
    template<class T>
    constexpr static AreaToPlantResult New(Acres area_to_plant, const Game<T> &game) noexcept;

    constexpr explicit operator Acres() const noexcept;

  private:
    constexpr explicit AreaToPlant(Acres area_to_plant) noexcept;

    Acres area_to_plant_;
};

struct RoundInput final {
    AreaToBuy area_to_buy;
    AreaToSell area_to_sell;
    GrainToFeed grain_to_feed;
    AreaToPlant area_to_plant;
};

struct Continue final {};

class GameOver final {
  public:
    constexpr explicit GameOver(People dead_from_hunger) noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

  private:
    People dead_from_hunger_;
};

class EndStatistics final {
  public:
    constexpr explicit EndStatistics(Acres area, People population, People dead_from_hunger) noexcept;

    [[nodiscard]]
    constexpr People AverageDeadFromHungerPercent() const noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

    [[nodiscard]]
    constexpr Acres AreaByPerson() const noexcept;

    enum class Rank : std::uint8_t {
        D = 2, C, B, A,
    };

    [[nodiscard]]
    constexpr Rank CalculateRank() const noexcept;

  private:
    People average_dead_from_hunger_percent_;
    People dead_from_hunger_;
    Acres area_by_person_;
};

using RoundResult = std::variant<Continue, GameOver, EndStatistics>;

template<class T>
class Game final {
  public:
    Game(Game &&other) = default;
    Game &operator=(Game &&other) = default;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    explicit Game(T generator);

    [[nodiscard]]
    constexpr Round CurrentRound() const noexcept;

    [[nodiscard]]
    constexpr People Population() const noexcept;

    [[nodiscard]]
    constexpr Acres Area() const noexcept;

    [[nodiscard]]
    constexpr Bushels Grain() const noexcept;

    [[nodiscard]]
    constexpr Bushels AcrePrice() const noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

    [[nodiscard]]
    constexpr People Arrived() const noexcept;

    [[nodiscard]]
    constexpr Bushels GrainFromAcre() const noexcept;

    [[nodiscard]]
    constexpr Bushels GrainEatenByRats() const noexcept;

    [[nodiscard]]
    constexpr bool IsPlague() const noexcept;

    [[nodiscard("result should be presented to the user")]]
    RoundResult PlayRound(RoundInput input);

  private:
    People population_;
    Acres area_;
    Bushels grain_;
    Bushels acre_price_;
    People dead_from_hunger_;
    People dead_from_hunger_in_total_;
    People arrived_;
    Bushels grain_from_acre_;
    Bushels grain_eaten_by_rats_;
    Round current_round_;
    bool is_plague_;
    bool is_game_over_;
    T generator_;
};

namespace detail {

constexpr Round kLastRound = 10;
constexpr Acres kAreaCanPlantWithBushel = 2;

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

[[nodiscard("result is used later to change game state")]]
constexpr Bushels CountGrainToPlantArea(const Acres area) noexcept {
    return area / kAreaCanPlantWithBushel;
}

struct FeedPeopleResult final {
    Bushels grain_left{0};
    People dead{0};
};

[[nodiscard("result is used later to change game state")]]
constexpr FeedPeopleResult FeedPeople(const People population, const Bushels grain_to_feed) noexcept {
    constexpr auto kGrainPerPerson = 20;

    const auto needed_grain = population * kGrainPerPerson;
    if (needed_grain > grain_to_feed) {
        const auto dead = (needed_grain - grain_to_feed - 1) / kGrainPerPerson + 1;
        return {.dead = dead};
    }
    const auto grain_left = grain_to_feed - needed_grain;
    return {.grain_left = grain_left};
}

[[nodiscard("it is important to track if the game is over")]]
constexpr bool IsGameOver(const People dead, const People population) noexcept {
    const auto percentage = (dead * 100) / population;
    return percentage > 45;
}

[[nodiscard("result is used later to change game state")]]
constexpr People CountArrivedPeople(const People dead,
                                    const Bushels harvested_from_acre,
                                    const Bushels grain) noexcept {
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

constexpr NotEnoughPeople::NotEnoughPeople(People population) noexcept: population_{population} {}

constexpr People NotEnoughPeople::Population() const noexcept {
    return population_;
}

constexpr GameOver::GameOver(People dead_from_hunger) noexcept: dead_from_hunger_{dead_from_hunger} {}

constexpr People GameOver::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr EndStatistics::EndStatistics(const Acres area,
                                       const People population,
                                       const People dead_from_hunger) noexcept
    : average_dead_from_hunger_percent_{dead_from_hunger / detail::kLastRound},
      dead_from_hunger_{dead_from_hunger},
      area_by_person_{area / population} {}

constexpr People EndStatistics::AverageDeadFromHungerPercent() const noexcept {
    return average_dead_from_hunger_percent_;
}

constexpr People EndStatistics::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

constexpr Acres EndStatistics::AreaByPerson() const noexcept {
    return area_by_person_;
}

constexpr EndStatistics::Rank EndStatistics::CalculateRank() const noexcept {
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
    constexpr Acres kAreaToPlantPerPerson = 10;

    if (area_to_plant > game.Area()) {
        return NotEnoughArea{game.Area()};
    }
    if (area_to_plant > game.Grain() * detail::kAreaCanPlantWithBushel) {
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
        return EndStatistics{area_, population_, dead_from_hunger_in_total_};
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
    const Bushels grain_to_plant_area = detail::CountGrainToPlantArea(area_to_plant);
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
        return EndStatistics{area_, population_, dead_from_hunger_in_total_};
    }
    return Continue{};
}

}

#endif //HAMURABI_GAME_H_
