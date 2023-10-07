#ifndef HAMURABI_SERIALIZATION
#define HAMURABI_SERIALIZATION

#include <istream>
#include <ostream>

#include "Game.fwd"

namespace hamurabi::serialization {

enum class Format : std::uint8_t {
    YAML,
};

template<class T>
void InsertGame(std::ostream &ostream, const Game<T> &game, Format format);

enum class ExtractResult : std::uint8_t {
    Success,
    Error,
};

template<class T>
[[nodiscard]]
ExtractResult ExtractGame(std::istream &istream, Game<T> &game, Format format);

}

#endif //HAMURABI_SERIALIZATION
