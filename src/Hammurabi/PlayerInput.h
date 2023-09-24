#ifndef HAMMURABI_PLAYER_INPUT_H_
#define HAMMURABI_PLAYER_INPUT_H_

#include "Resources.h"

namespace hammurabi {

struct PlayerInput {
    Acres area_to_buy{};
    Acres area_to_sell{};
    Bushels grain_to_feed{};
    Bushels grain_to_plant{};
};

}

#endif //HAMMURABI_PLAYER_INPUT_H_
