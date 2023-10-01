#ifndef HAMURABI_GAME_DECL_H_
#define HAMURABI_GAME_DECL_H_

#include <variant>
#include <optional>

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

class GameOver final {
  public:
    constexpr explicit GameOver(People dead_from_hunger) noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

  private:
    People dead_from_hunger_;
};

struct Continue final {};

struct GameEnd final {};

class GameStatistics final {
  public:
    constexpr explicit GameStatistics(Acres area, People population, People dead_from_hunger) noexcept;

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

using RoundResult = std::variant<Continue, GameOver, GameEnd>;

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

    [[nodiscard("result should be presented to the user")]]
    std::optional<GameStatistics> Statistics() const noexcept;

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

}

#endif //HAMURABI_GAME_DECL_H_
