#ifndef PLAY_HAMURABI
#define PLAY_HAMURABI

#include "Detail.hpp"

namespace play {

template<class T>
void Hamurabi(std::istream &istream, std::ostream &ostream, hamurabi::Game<T> game);

}

#include "Hamurabi.inl"

#endif //PLAY_HAMURABI
