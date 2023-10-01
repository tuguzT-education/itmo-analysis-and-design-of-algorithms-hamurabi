#ifndef HAMURABI_GAME_OVER
#define HAMURABI_GAME_OVER

#include "Resources.hpp"
#include "Game.fwd"

namespace hamurabi {

class GameOver final {
  public:
    template<class T>
    constexpr explicit GameOver(const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr People DeadFromHunger() const noexcept;

  private:
    People dead_from_hunger_;
};

}

#include "GameOver.inl"

#endif //HAMURABI_GAME_OVER
