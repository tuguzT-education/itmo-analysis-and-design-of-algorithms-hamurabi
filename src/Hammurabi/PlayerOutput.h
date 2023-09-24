#ifndef HAMMURABI_PLAYER_OUTPUT_H_
#define HAMMURABI_PLAYER_OUTPUT_H_

#include "Resources.h"

namespace hammurabi {

struct PlayerOutput {
    People dead_people_count{};
    People new_people_count{};
    People population{};
    Bushels grain_harvested{};
    Bushels grain_from_acre{};
    Bushels grain_eaten_by_rats{};
    Acres area{};
    Bushels acre_price{};
    Round current_round{};
    bool is_plague{};
};

}

#endif //HAMMURABI_PLAYER_OUTPUT_H_
