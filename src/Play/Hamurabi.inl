#ifndef PLAY_HAMURABI_INL
#define PLAY_HAMURABI_INL

namespace play {

void InsertGreetings(std::ostream &ostream) {
    ostream << "                                HAMURABI\n"
               "               CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY\n\n"
               "TRY YOUR HAND AT GOVERNING ANCIENT SUMERIA\n"
               "FOR A TEN-YEAR TERM OF OFFICE.\n\n";
}

void InsertNotEnoughArea(std::ostream &ostream, const hamurabi::NotEnoughArea error) {
    ostream << "HAMURABI: THINK AGAIN. YOU OWN ONLY "
            << error.Area() << " ACRES. NOW THEN,\n";
}

void InsertNotEnoughGrain(std::ostream &ostream, const hamurabi::NotEnoughGrain error) {
    ostream << "HAMURABI: THINK AGAIN. YOU HAVE ONLY\n"
            << error.Grain() << " BUSHELS OF GRAIN. NOW THEN,\n";
}

void InsertNotEnoughPeople(std::ostream &ostream, const hamurabi::NotEnoughPeople error) {
    ostream << "HAMURABI: THINK AGAIN. YOU HAVE ONLY "
            << error.Population() << " PEOPLE TO TEND THE FIELDS! NOW THEN,\n";
}

void InsertGameOver(std::ostream &ostream, const hamurabi::GameOver game_over) {
    ostream << "YOU STARVED " << game_over.DeadFromHunger() << " PEOPLE IN ONE YEAR!!!\n"
            << "DUE TO THIS EXTREME MISMANAGEMENT YOU HAVE NOT ONLY\n"
               "BEEN IMPEACHED AND THROWN OUT OF OFFICE BUT YOU HAVE\n"
               "ALSO BEEN DECLARED NATIONAL FINK!!!!\n";
}

template<class T>
void InsertGameState(std::ostream &ostream, const hamurabi::Game<T> &game) {
    ostream << "HAMURABI:  I BEG TO REPORT TO YOU,\n"
               "IN YEAR " << game.CurrentRound() << ",";
    if (game.DeadFromHunger() > 0) {
        ostream << " " << game.DeadFromHunger() << " PEOPLE STARVED,";
    }
    if (game.Arrived() > 0) {
        ostream << " " << game.Arrived() << " PEOPLE CAME TO THE CITY,";
    }
    ostream << "\n";

    if (game.IsPlague()) {
        ostream << "A HORRIBLE PLAGUE STRUCK!  HALF THE PEOPLE DIED.\n";
    }
    ostream << "POPULATION IS NOW " << game.Population() << ".\n"
            << "THE CITY NOW OWNS " << game.Area() << " ACRES.\n"
            << "YOU HARVESTED " << game.GrainFromAcre() << " BUSHELS PER ACRE.\n";
    if (game.GrainEatenByRats() > 0) {
        ostream << "RATS ATE " << game.GrainEatenByRats() << " BUSHELS.\n";
    }
    ostream << "YOU NOW HAVE " << game.Grain() << " BUSHELS IN STORE.\n"
            << "LAND IS TRADING AT " << game.AcrePrice() << " BUSHELS PER ACRE.\n";
}

void InsertGameStatistics(std::ostream &ostream, const hamurabi::Statistics statistics) {
    ostream << "IN YOUR 10-YEAR TERM OF OFFICE, " << statistics.AverageDeadFromHungerPercent() << " PERCENT OF THE\n"
            << "POPULATION STARVED PER YEAR ON THE AVERAGE, I.E. A TOTAL OF\n"
            << statistics.DeadFromHunger() << " PEOPLE DIED!!\n"
            << "YOU STARTED WITH 10 ACRES PER PERSON AND ENDED WITH\n"
            << statistics.AreaByPerson() << " ACRES PER PERSON\n";

    const auto rank = statistics.Rank();
    switch (rank) {
        case hamurabi::Rank::D: {
            ostream << "THE PEOPLE (REMAINING) FIND YOU AN UNPLEASANT RULER, AND,\n"
                       "FRANKLY, HATE YOUR GUTS!\n";
            break;
        }
        case hamurabi::Rank::C: {
            ostream << "YOUR HEAVY-HANDED PERFORMANCE SMACKS OF NERO AND IVAN IV.\n";
            break;
        }
        case hamurabi::Rank::B: {
            ostream << "YOUR PERFORMANCE COULD HAVE BEEN SOMEWHAT BETTER, BUT\n"
                       "REALLY WASN'T TOO BAD AT ALL. PEOPLE\n"
                       "DEARLY LIKE TO SEE YOU ASSASSINATED BUT WE ALL HAVE OUR\n"
                       "TRIVIAL PROBLEMS.\n";
            break;
        }
        case hamurabi::Rank::A: {
            ostream << "A FANTASTIC PERFORMANCE!!! CHARLEMAGNE, DISRAELI, AND\n"
                       "JEFFERSON COMBINED COULD NOT HAVE DONE BETTER!\n";
            break;
        }
    }
}

void InsertGoodbye(std::ostream &ostream) {
    ostream << "\nSO LONG FOR NOW.\n";
}

template<std::unsigned_integral T>
T ExtractUnsigned(std::istream &istream, std::ostream &ostream, const std::string_view message) {
    const auto error_message = "HAMURABI: I CANNOT DO WHAT YOU WISH.  NOW THEN,\n";
    std::make_signed_t<T> value;
    while (true) {
        ostream << message;
        istream >> value;
        if (istream.fail()) {
            istream.clear();
            istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ostream << error_message;
            continue;
        }
        if (value < 0) {
            ostream << error_message;
            continue;
        }
        return static_cast<T>(value);
    }
}

namespace detail {

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}

template<class T>
hamurabi::AreaToBuy ExtractAreaToBuy(std::istream &istream, std::ostream &ostream,
                                     const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToBuyResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<decltype(input)>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO BUY? ");
        result = hamurabi::AreaToBuy::New(input, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::AreaToBuy) { try_again = false; },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToBuy>(result.value());
}

template<class T>
hamurabi::AreaToSell ExtractAreaToSell(std::istream &istream, std::ostream &ostream,
                                       const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToSellResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<decltype(input)>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO SELL? ");
        result = hamurabi::AreaToSell::New(input, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::AreaToSell) { try_again = false; },
            [&ostream](hamurabi::NotEnoughArea error) { InsertNotEnoughArea(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToSell>(result.value());
}

template<class T>
hamurabi::GrainToFeed ExtractGrainToFeed(std::istream &istream, std::ostream &ostream,
                                         const hamurabi::Game<T> &game) {
    hamurabi::Bushels input;
    std::optional<hamurabi::GrainToFeedResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<decltype(input)>(
            istream, ostream, "HOW MANY BUSHELS DO YOU WISH TO FEED YOUR PEOPLE? ");
        result = hamurabi::GrainToFeed::New(input, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::GrainToFeed) { try_again = false; },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::GrainToFeed>(result.value());
}

template<class T>
hamurabi::AreaToPlant ExtractAreaToPlant(std::istream &istream, std::ostream &ostream,
                                         const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToPlantResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<decltype(input)>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO PLANT WITH SEED? ");
        result = hamurabi::AreaToPlant::New(input, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::AreaToPlant) { try_again = false; },
            [&ostream](hamurabi::NotEnoughArea error) { InsertNotEnoughArea(ostream, error); },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
            [&ostream](hamurabi::NotEnoughPeople error) { InsertNotEnoughPeople(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToPlant>(result.value());
}

template<class T>
hamurabi::RoundInput ExtractRoundInput(std::istream &istream, std::ostream &ostream,
                                       const hamurabi::Game<T> &game) {
    std::optional<hamurabi::RoundInputResult> result;
    bool try_again = true;
    while (try_again) {
        const auto area_to_buy = ExtractAreaToBuy(istream, ostream, game);
        const auto area_to_sell = ExtractAreaToSell(istream, ostream, game);
        const auto grain_to_feed = ExtractGrainToFeed(istream, ostream, game);
        const auto area_to_plant = ExtractAreaToPlant(istream, ostream, game);
        result = hamurabi::RoundInput::New(area_to_buy, area_to_sell, grain_to_feed, area_to_plant, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::RoundInput) { try_again = false; },
            [&ostream](hamurabi::NotEnoughArea error) { InsertNotEnoughArea(ostream, error); },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
            [&ostream](hamurabi::NotEnoughPeople error) { InsertNotEnoughPeople(ostream, error); },
        }, result.value());
    }
    ostream << "\n";
    return std::get<hamurabi::RoundInput>(result.value());
}

template<class T>
void Hamurabi(std::istream &istream, std::ostream &ostream, hamurabi::Game<T> game) {
    InsertGreetings(ostream);
    InsertGameState(ostream, game);

    bool can_play = true;
    while (can_play) {
        hamurabi::RoundInput input = ExtractRoundInput(istream, ostream, game);
        hamurabi::RoundResult result = game.PlayRound(input);
        std::visit(detail::overloaded{
            [&ostream, &can_play](hamurabi::GameOver game_over) {
                InsertGameOver(ostream, game_over);
                can_play = false;
            },
            [&ostream, &game = std::as_const(game)](hamurabi::Continue) {
                InsertGameState(ostream, game);
            },
            [&ostream, &game = std::as_const(game), &can_play](hamurabi::GameEnd) {
                const auto statistics = game.Statistics().value();
                InsertGameState(ostream, game);
                InsertGameStatistics(ostream, statistics);
                can_play = false;
            },
        }, result);
    }

    InsertGoodbye(ostream);
}

}

#endif //PLAY_HAMURABI_INL
