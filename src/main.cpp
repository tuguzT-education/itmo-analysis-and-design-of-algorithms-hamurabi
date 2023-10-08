#include <iostream>

#include "Play/Hamurabi.hpp"

int main() {
    std::random_device random_device{};
    std::mt19937_64 generator{random_device()};
    hamurabi::Game game{generator};
    std::fstream file{};
    play::Hamurabi(std::cin, std::cout, file, game);
    return 0;
}
