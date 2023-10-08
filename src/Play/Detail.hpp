#ifndef PLAY_DETAIL
#define PLAY_DETAIL

#include "../Hamurabi/Game.hpp"

#include <fstream>

namespace play::detail {

static inline void InsertGreetings(std::ostream &ostream);

static inline void InsertNotEnoughArea(std::ostream &ostream, hamurabi::NotEnoughArea error);

static inline void InsertNotEnoughGrain(std::ostream &ostream, hamurabi::NotEnoughGrain error);

static inline void InsertNotEnoughPeople(std::ostream &ostream, hamurabi::NotEnoughPeople error);

static inline void InsertGameOver(std::ostream &ostream, hamurabi::GameOver game_over);

template<class T>
static inline void InsertGameState(std::ostream &ostream, const hamurabi::Game<T> &game);

static inline void InsertGameStatistics(std::ostream &ostream, hamurabi::Statistics statistics);

static inline void InsertGoodbye(std::ostream &ostream);

static inline void InsertOldGameFound(std::ostream &ostream);

extern const hamurabi::string_literal kExitCommand;

static inline constexpr bool CanExit(std::string_view string) noexcept;

struct Exit final {};

template<class T>
using ExitOr = std::variant<Exit, T>;

template<std::unsigned_integral T>
[[nodiscard]]
static inline ExitOr<T> ExtractUnsigned(std::istream &istream, std::ostream &ostream,
                                        std::string_view message);

template<class T>
[[nodiscard]]
static inline ExitOr<hamurabi::AreaToBuy> ExtractAreaToBuy(std::istream &istream, std::ostream &ostream,
                                                           const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
static inline ExitOr<hamurabi::AreaToSell> ExtractAreaToSell(std::istream &istream, std::ostream &ostream,
                                                             const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
static inline ExitOr<hamurabi::GrainToFeed> ExtractGrainToFeed(std::istream &istream, std::ostream &ostream,
                                                               const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
static inline ExitOr<hamurabi::AreaToPlant> ExtractAreaToPlant(std::istream &istream, std::ostream &ostream,
                                                               const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
static inline ExitOr<hamurabi::RoundInput> ExtractRoundInput(std::istream &istream, std::ostream &ostream,
                                                             const hamurabi::Game<T> &game);

extern const hamurabi::string_literal kSaveFileName;

template<class T>
static inline void InsertGame(std::fstream &file, const hamurabi::Game<T> &game);

template<class T>
[[nodiscard]]
static inline hamurabi::ser::ExtractResult ExtractGame(std::istream &istream, std::ostream &ostream,
                                                       std::fstream &file, hamurabi::Game<T> &game);

enum class ContinueOrStartNew {
    Continue,
    StartNew,
};

extern const hamurabi::string_literal kContinueCommand;

static inline constexpr bool CanContinue(std::string_view string) noexcept;

extern const hamurabi::string_literal kStartNewCommand;

static inline constexpr bool CanStartNew(std::string_view string) noexcept;

[[nodiscard]]
static inline ContinueOrStartNew ExtractContinueOrStartNew(std::istream &istream, std::ostream &ostream);

}

#include "Detail.inl"

#endif //PLAY_DETAIL
