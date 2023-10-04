#ifndef PLAY_HAMURABI
#define PLAY_HAMURABI

#include <istream>
#include <ostream>
#include <string_view>

#include "../Hamurabi/Game.hpp"

namespace play {

void InsertGreetings(std::ostream &ostream);

void InsertNotEnoughArea(std::ostream &ostream, hamurabi::NotEnoughArea error);

void InsertNotEnoughGrain(std::ostream &ostream, hamurabi::NotEnoughGrain error);

void InsertNotEnoughPeople(std::ostream &ostream, hamurabi::NotEnoughPeople error);

void InsertGameOver(std::ostream &ostream, hamurabi::GameOver game_over);

template<class T>
void InsertGameState(std::ostream &ostream, const hamurabi::Game<T> &game);

void InsertGameStatistics(std::ostream &ostream, hamurabi::Statistics statistics);

void InsertGoodbye(std::ostream &ostream);

template<std::unsigned_integral T>
T ExtractUnsigned(std::istream &istream, std::ostream &ostream, std::string_view message);

template<class T>
hamurabi::AreaToBuy ExtractAreaToBuy(std::istream &istream, std::ostream &ostream,
                                     const hamurabi::Game<T> &game);

template<class T>
hamurabi::AreaToSell ExtractAreaToSell(std::istream &istream, std::ostream &ostream,
                                       const hamurabi::Game<T> &game);

template<class T>
hamurabi::GrainToFeed ExtractGrainToFeed(std::istream &istream, std::ostream &ostream,
                                         const hamurabi::Game<T> &game);

template<class T>
hamurabi::AreaToPlant ExtractAreaToPlant(std::istream &istream, std::ostream &ostream,
                                         const hamurabi::Game<T> &game);

template<class T>
hamurabi::RoundInput ExtractRoundInput(std::istream &istream, std::ostream &ostream,
                                       const hamurabi::Game<T> &game);

template<class T>
void Hamurabi(std::istream &istream, std::ostream &ostream, hamurabi::Game<T> game);

}

#include "Hamurabi.inl"

#endif //PLAY_HAMURABI
