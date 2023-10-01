#ifndef HAMURABI_NOT_ENOUGH_PEOPLE
#define HAMURABI_NOT_ENOUGH_PEOPLE

#include "Resources.hpp"
#include "Game.fwd"

namespace hamurabi {

class NotEnoughPeople final {
  public:
    template<class T>
    constexpr explicit NotEnoughPeople(const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr People Population() const noexcept;

  private:
    People population_;
};

}

#include "NotEnoughPeople.inl"

#endif //HAMURABI_NOT_ENOUGH_PEOPLE
