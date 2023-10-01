#ifndef HAMURABI_ROUND_INPUT
#define HAMURABI_ROUND_INPUT

#include <variant>

#include "AreaToBuy.hpp"
#include "AreaToSell.hpp"
#include "GrainToFeed.hpp"
#include "AreaToPlant.hpp"

namespace hamurabi {

class RoundInput;
using RoundInputResult = std::variant<RoundInput, NotEnoughArea, NotEnoughGrain, NotEnoughPeople>;

class RoundInput final {
  public:
    template<class T>
    constexpr static RoundInputResult New(AreaToBuy area_to_buy,
                                          AreaToSell area_to_sell,
                                          GrainToFeed grain_to_feed,
                                          AreaToPlant area_to_plant,
                                          const Game<T> &game) noexcept;

    [[nodiscard]]
    constexpr AreaToBuy AreaToBuy() const;

    [[nodiscard]]
    constexpr AreaToSell AreaToSell() const;

    [[nodiscard]]
    constexpr GrainToFeed GrainToFeed() const;

    [[nodiscard]]
    constexpr AreaToPlant AreaToPlant() const;

  private:
    constexpr explicit RoundInput(hamurabi::AreaToBuy area_to_buy,
                                  hamurabi::AreaToSell area_to_sell,
                                  hamurabi::GrainToFeed grain_to_feed,
                                  hamurabi::AreaToPlant area_to_plant) noexcept;

    hamurabi::AreaToBuy area_to_buy_;
    hamurabi::AreaToSell area_to_sell_;
    hamurabi::GrainToFeed grain_to_feed_;
    hamurabi::AreaToPlant area_to_plant_;
};

}

#include "RoundInput.inl"

#endif //HAMURABI_ROUND_INPUT
