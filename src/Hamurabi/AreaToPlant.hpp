#ifndef HAMURABI_AREA_TO_PLANT
#define HAMURABI_AREA_TO_PLANT

#include <variant>

#include "NotEnoughArea.hpp"
#include "NotEnoughGrain.hpp"
#include "NotEnoughPeople.hpp"
#include "Detail.hpp"

namespace hamurabi {

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

}

#include "AreaToPlant.inl"

#endif //HAMURABI_AREA_TO_PLANT
