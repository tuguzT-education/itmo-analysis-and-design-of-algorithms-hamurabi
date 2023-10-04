#include <iostream>

#include "Play/Hamurabi.hpp"

int main() {
    std::random_device random_device;
    std::mt19937_64 generator{random_device()};
    hamurabi::Game game{generator};
    play::Hamurabi(std::cin, std::cout, std::move(game));
    return 0;
}
