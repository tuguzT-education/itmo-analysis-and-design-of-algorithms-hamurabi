#ifndef HAMURABI_NOT_ENOUGH_AREA
#define HAMURABI_NOT_ENOUGH_AREA

#include "Resources.hpp"
#include "Game.fwd"

namespace hamurabi {

class NotEnoughArea final {
  public:
    template<class T>
    constexpr explicit NotEnoughArea(const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr Acres Area() const noexcept;

  private:
    Acres area_;
};

}

#include "NotEnoughArea.inl"

#endif //HAMURABI_NOT_ENOUGH_AREA
