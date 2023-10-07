#ifndef PLAY_DETAIL
#define PLAY_DETAIL

#include "../Hamurabi/Game.hpp"

namespace play::detail {

void InsertGreetings(std::ostream &ostream);

void InsertNotEnoughArea(std::ostream &ostream, hamurabi::NotEnoughArea error);

void InsertNotEnoughGrain(std::ostream &ostream, hamurabi::NotEnoughGrain error);

void InsertNotEnoughPeople(std::ostream &ostream, hamurabi::NotEnoughPeople error);

void InsertGameOver(std::ostream &ostream, hamurabi::GameOver game_over);

template<class T>
void InsertGameState(std::ostream &ostream, const hamurabi::Game<T> &game);

void InsertGameStatistics(std::ostream &ostream, hamurabi::Statistics statistics);

void InsertGoodbye(std::ostream &ostream);

extern const hamurabi::string_literal kExitCommand;

constexpr bool CanExit(std::string_view string) noexcept;

struct Exit final {};

template<class T>
using ExitOr = std::variant<Exit, T>;

template<std::unsigned_integral T>
[[nodiscard]]
ExitOr<T> ExtractUnsigned(std::istream &istream, std::ostream &ostream, std::string_view message);

template<class T>
[[nodiscard]]
ExitOr<hamurabi::AreaToBuy> ExtractAreaToBuy(std::istream &istream, std::ostream &ostream,
                                             const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
ExitOr<hamurabi::AreaToSell> ExtractAreaToSell(std::istream &istream, std::ostream &ostream,
                                               const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
ExitOr<hamurabi::GrainToFeed> ExtractGrainToFeed(std::istream &istream, std::ostream &ostream,
                                                 const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
ExitOr<hamurabi::AreaToPlant> ExtractAreaToPlant(std::istream &istream, std::ostream &ostream,
                                                 const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
ExitOr<hamurabi::RoundInput> ExtractRoundInput(std::istream &istream, std::ostream &ostream,
                                               const hamurabi::Game<T> &game);

}

#include "Detail.inl"

#endif //PLAY_DETAIL
