#ifndef HAMMURABI_CITY_H_
#define HAMMURABI_CITY_H_

#include "Resources.h"

namespace hammurabi {

struct City {
    People population{100};
    Acres area{1000};
    Bushels grain{2800};

    constexpr explicit City() noexcept = default;
};

}

#endif //HAMMURABI_CITY_H_
