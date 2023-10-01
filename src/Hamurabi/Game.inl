#ifndef HAMURABI_GAME_INL
#define HAMURABI_GAME_INL

namespace hamurabi {

template<class T>
Game<T>::Game(T generator)
    : generator_{generator},
      current_round_{1},
      population_{100},
      area_{1000},
      grain_{2800},
      dead_from_hunger_{0},
      dead_from_hunger_in_total_{0},
      arrived_{5},
      grain_from_acre_{3},
      grain_eaten_by_rats_{200},
      is_plague_{false},
      is_game_over_{false} {
    acre_price_ = detail::GenerateAcrePrice(generator_);
}

template<class T>
constexpr Round Game<T>::CurrentRound() const noexcept {
    return current_round_;
}

template<class T>
constexpr People Game<T>::Population() const noexcept {
    return population_;
}

template<class T>
constexpr Acres Game<T>::Area() const noexcept {
    return area_;
}

template<class T>
constexpr Bushels Game<T>::Grain() const noexcept {
    return grain_;
}

template<class T>
constexpr Bushels Game<T>::AcrePrice() const noexcept {
    return acre_price_;
}

template<class T>
constexpr People Game<T>::DeadFromHunger() const noexcept {
    return dead_from_hunger_;
}

template<class T>
constexpr People Game<T>::DeadFromHungerInTotal() const noexcept {
    return dead_from_hunger_in_total_;
}

template<class T>
constexpr People Game<T>::Arrived() const noexcept {
    return arrived_;
}

template<class T>
constexpr Bushels Game<T>::GrainFromAcre() const noexcept {
    return grain_from_acre_;
}

template<class T>
constexpr Bushels Game<T>::GrainEatenByRats() const noexcept {
    return grain_eaten_by_rats_;
}

template<class T>
constexpr bool Game<T>::IsPlague() const noexcept {
    return is_plague_;
}

template<class T>
RoundResult Game<T>::PlayRound(const RoundInput input) {
    if (is_game_over_) {
        return GameOver{*this};
    }
    if (current_round_ > detail::kLastRound) {
        return GameEnd{};
    }
    current_round_ += 1;

    const auto area_to_buy = static_cast<Acres>(input.AreaToBuy());
    area_ += area_to_buy;
    const Bushels grain_to_buy_area = area_to_buy * acre_price_;
    grain_ -= grain_to_buy_area;

    const auto area_to_sell = static_cast<Acres>(input.AreaToSell());
    area_ -= area_to_sell;
    const Bushels grain_to_sell_area = area_to_sell * acre_price_;
    grain_ += grain_to_sell_area;

    const auto area_to_plant = static_cast<Acres>(input.AreaToPlant());
    grain_from_acre_ = detail::GenerateGrainHarvestedFromAcre(generator_);
    const Bushels grain_harvested = area_to_plant * grain_from_acre_;
    grain_ += grain_harvested;
    const Bushels grain_to_plant_area = detail::GrainToPlantArea(area_to_plant);
    grain_ -= grain_to_plant_area;

    const auto grain_to_feed = static_cast<Bushels>(input.GrainToFeed());
    const auto feed_people_result = detail::FeedPeople(population_, grain_to_feed);
    grain_ += feed_people_result.grain_left;
    grain_ -= grain_to_feed;
    const auto old_population = population_;
    dead_from_hunger_ = feed_people_result.dead;
    population_ -= dead_from_hunger_;
    dead_from_hunger_in_total_ += dead_from_hunger_;
    if (detail::IsGameOver(dead_from_hunger_, old_population)) {
        is_game_over_ = true;
        return GameOver{*this};
    }

    grain_eaten_by_rats_ = detail::GenerateGrainEatenByRats(generator_, grain_);
    grain_ -= grain_eaten_by_rats_;

    arrived_ = detail::CountArrivedPeople(dead_from_hunger_, grain_from_acre_, grain_);
    population_ += arrived_;

    is_plague_ = detail::GenerateIsPlague(generator_);
    if (is_plague_) {
        population_ /= 2;
    }

    acre_price_ = detail::GenerateAcrePrice(generator_);
    if (current_round_ > detail::kLastRound) {
        return GameEnd{};
    }
    return Continue{};
}

template<class T>
std::optional<Statistics> Game<T>::Statistics() const noexcept {
    if (current_round_ > detail::kLastRound) {
        return hamurabi::Statistics{*this};
    }
    return std::nullopt;
}

}

#endif //HAMURABI_GAME_INL
