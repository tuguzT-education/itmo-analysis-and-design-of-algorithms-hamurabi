#ifndef HAMURABI_GAME_OVER_INL
#define HAMURABI_GAME_OVER_INL

namespace hamurabi {

template<class T>
constexpr GameOver::GameOver(const Game<T> &game) noexcept
    : dead_from_hunger_{game.DeadFromHunger()} {}

constexpr People GameOver::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

}

#endif //HAMURABI_GAME_OVER_INL
