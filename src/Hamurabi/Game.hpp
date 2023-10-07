#ifndef HAMURABI_GAME
#define HAMURABI_GAME

#include <variant>
#include <optional>
#include <istream>
#include <ostream>

#include "NotEnoughArea.hpp"
#include "NotEnoughGrain.hpp"
#include "NotEnoughPeople.hpp"
#include "AreaToBuy.hpp"
#include "AreaToSell.hpp"
#include "GrainToFeed.hpp"
#include "AreaToPlant.hpp"
#include "RoundInput.hpp"
#include "Continue.hpp"
#include "GameOver.hpp"
#include "GameEnd.hpp"
#include "Statistics.hpp"
#include "Detail.hpp"

namespace hamurabi {

using RoundResult = std::variant<Continue, GameOver, GameEnd>;

namespace ser = serialization;

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
    constexpr People DeadFromHungerInTotal() const noexcept;

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
    std::optional<Statistics> Statistics() const noexcept;

    friend void ser::InsertGame<T>(std::ostream &ostream, const Game<T> &game, ser::Format format);

    friend ser::ExtractResult ser::ExtractGame<T>(std::istream &istream, Game<T> &game, ser::Format format);

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

#include "Game.inl"

#endif //HAMURABI_GAME
