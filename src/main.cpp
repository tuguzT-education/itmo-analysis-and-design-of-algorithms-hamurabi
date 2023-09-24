#include "Hammurabi/City.h"
#include "Hammurabi/PlayerInput.h"
#include "Hammurabi/PlayerOutput.h"
#include "Hammurabi/Game.h"

hammurabi::PlayerInput ProcessPlayerInput() {
    hammurabi::PlayerInput player_input{};
    return player_input;
}

int main() {
    hammurabi::City city;
    for (hammurabi::Round round = 1; round <= 10; ++round) {
        const auto player_input = ProcessPlayerInput();
    }
    return 0;
}
