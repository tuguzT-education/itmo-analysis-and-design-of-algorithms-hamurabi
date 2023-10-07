#ifndef PLAY_DETAIL_INL
#define PLAY_DETAIL_INL

namespace play::detail {

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

constexpr hamurabi::string_literal kExitCommand = "exit";

constexpr bool CanExit(const std::string_view string) noexcept {
    return hamurabi::detail::Trim(string) == kExitCommand;
}

template<std::unsigned_integral T>
ExitOr<T> ExtractUnsigned(std::istream &istream, std::ostream &ostream, const std::string_view message) {
    const auto error_message = "HAMURABI: I CANNOT DO WHAT YOU WISH.  NOW THEN,\n";
    std::make_signed_t<T> value;
    std::string buffer;

    while (true) {
        // prints message
        ostream << message;
        // gets new line and check for exit
        std::getline(istream, buffer);
        if (CanExit(buffer)) {
            return Exit{};
        }
        // converts input into integer
        try {
            value = std::stoll(buffer);
        } catch (const std::logic_error &) {
            ostream << error_message;
            continue;
        }
        // checks for negative values
        if (value < 0) {
            ostream << error_message;
            continue;
        }
        return static_cast<T>(value);
    }
}

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

template<class T>
ExitOr<hamurabi::AreaToBuy> ExtractAreaToBuy(std::istream &istream, std::ostream &ostream,
                                             const hamurabi::Game<T> &game) {
    ExitOr<hamurabi::Acres> input;
    std::optional<hamurabi::AreaToBuyResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<hamurabi::Acres>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO BUY? ");
        if (std::holds_alternative<Exit>(input)) {
            return Exit{};
        }
        const auto area_to_buy = std::get<hamurabi::Acres>(input);
        result = hamurabi::AreaToBuy::New(area_to_buy, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::AreaToBuy) { try_again = false; },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToBuy>(result.value());
}

template<class T>
ExitOr<hamurabi::AreaToSell> ExtractAreaToSell(std::istream &istream, std::ostream &ostream,
                                               const hamurabi::Game<T> &game) {
    ExitOr<hamurabi::Acres> input;
    std::optional<hamurabi::AreaToSellResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<hamurabi::Acres>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO SELL? ");
        if (std::holds_alternative<Exit>(input)) {
            return Exit{};
        }
        const auto area_to_sell = std::get<hamurabi::Acres>(input);
        result = hamurabi::AreaToSell::New(area_to_sell, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::AreaToSell) { try_again = false; },
            [&ostream](hamurabi::NotEnoughArea error) { InsertNotEnoughArea(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToSell>(result.value());
}

template<class T>
ExitOr<hamurabi::GrainToFeed> ExtractGrainToFeed(std::istream &istream, std::ostream &ostream,
                                                 const hamurabi::Game<T> &game) {
    ExitOr<hamurabi::Bushels> input;
    std::optional<hamurabi::GrainToFeedResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<hamurabi::Bushels>(
            istream, ostream, "HOW MANY BUSHELS DO YOU WISH TO FEED YOUR PEOPLE? ");
        if (std::holds_alternative<Exit>(input)) {
            return Exit{};
        }
        const auto grain_to_feed = std::get<hamurabi::Bushels>(input);
        result = hamurabi::GrainToFeed::New(grain_to_feed, game);
        std::visit(detail::overloaded{
            [&try_again](hamurabi::GrainToFeed) { try_again = false; },
            [&ostream](hamurabi::NotEnoughGrain error) { InsertNotEnoughGrain(ostream, error); },
        }, result.value());
    }
    return std::get<hamurabi::GrainToFeed>(result.value());
}

template<class T>
ExitOr<hamurabi::AreaToPlant> ExtractAreaToPlant(std::istream &istream, std::ostream &ostream,
                                                 const hamurabi::Game<T> &game) {
    ExitOr<hamurabi::Acres> input;
    std::optional<hamurabi::AreaToPlantResult> result;
    bool try_again = true;
    while (try_again) {
        input = ExtractUnsigned<hamurabi::Acres>(
            istream, ostream, "HOW MANY ACRES DO YOU WISH TO PLANT WITH SEED? ");
        if (std::holds_alternative<Exit>(input)) {
            return Exit{};
        }
        const auto area_to_plant = std::get<hamurabi::Acres>(input);
        result = hamurabi::AreaToPlant::New(area_to_plant, game);
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
ExitOr<hamurabi::RoundInput> ExtractRoundInput(std::istream &istream, std::ostream &ostream,
                                               const hamurabi::Game<T> &game) {
    std::optional<hamurabi::RoundInputResult> result;
    bool try_again = true;
    while (try_again) {
        // if we got area to buy?
        const auto area_to_buy_or = ExtractAreaToBuy(istream, ostream, game);
        if (std::holds_alternative<Exit>(area_to_buy_or)) {
            return Exit{};
        }
        const auto area_to_buy = std::get<hamurabi::AreaToBuy>(area_to_buy_or);
        // if we got area to sell?
        const auto area_to_sell_or = ExtractAreaToSell(istream, ostream, game);
        if (std::holds_alternative<Exit>(area_to_sell_or)) {
            return Exit{};
        }
        const auto area_to_sell = std::get<hamurabi::AreaToSell>(area_to_sell_or);
        // if we got grain to feed our people?
        const auto grain_to_feed_or = ExtractGrainToFeed(istream, ostream, game);
        if (std::holds_alternative<Exit>(grain_to_feed_or)) {
            return Exit{};
        }
        const auto grain_to_feed = std::get<hamurabi::GrainToFeed>(grain_to_feed_or);
        // if we got area to plant our crops to?
        const auto area_to_plant_or = ExtractAreaToPlant(istream, ostream, game);
        if (std::holds_alternative<Exit>(area_to_plant_or)) {
            return Exit{};
        }
        const auto area_to_plant = std::get<hamurabi::AreaToPlant>(area_to_plant_or);
        // checks for round input result
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

}

#endif //PLAY_DETAIL_INL
