#ifndef HAMURABI_NOT_ENOUGH_GRAIN
#define HAMURABI_NOT_ENOUGH_GRAIN

#include "Resources.hpp"
#include "Game.fwd"

namespace hamurabi {

class NotEnoughGrain final {
  public:
    template<class T>
    constexpr explicit NotEnoughGrain(const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr Bushels Grain() const noexcept;

  private:
    Bushels grain_;
};

}

#include "NotEnoughGrain.inl"

#endif //HAMURABI_NOT_ENOUGH_GRAIN
