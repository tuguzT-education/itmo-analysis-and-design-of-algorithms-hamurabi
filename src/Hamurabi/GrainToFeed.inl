#ifndef HAMURABI_GRAIN_TO_FEED_INL
#define HAMURABI_GRAIN_TO_FEED_INL

namespace hamurabi {

template<class T>
constexpr GrainToFeedResult GrainToFeed::New(const Bushels grain_to_feed, const Game<T> &game) noexcept {
    const auto grain = game.Grain();
    if (grain_to_feed > grain) {
        return NotEnoughGrain{game};
    }
    return GrainToFeed{grain_to_feed};
}

constexpr GrainToFeed::operator Bushels() const noexcept {
    return grain_to_feed_;
}

constexpr GrainToFeed::GrainToFeed(const Bushels grain_to_feed) noexcept
    : grain_to_feed_{grain_to_feed} {}

}

#endif //HAMURABI_GRAIN_TO_FEED_INL
