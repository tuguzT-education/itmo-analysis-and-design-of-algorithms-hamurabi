#include "Hamurabi/Game.h"

#include <iostream>
#include <string_view>
#include <optional>

void PrintGreetings() {
    std::cout << "                                HAMURABI\n"
                 "               CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY\n\n"
                 "TRY YOUR HAND AT GOVERNING ANCIENT SUMERIA\n"
                 "FOR A TEN-YEAR TERM OF OFFICE.\n\n";
}

template<std::unsigned_integral T>
T ReceiveUnsigned(const std::string_view message) {
    const auto error_message = "HAMURABI: I CANNOT DO WHAT YOU WISH.  NOW THEN,\n";
    std::make_signed_t<T> value;
    while (true) {
        std::cout << message;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << error_message;
            continue;
        }
        if (value < 0) {
            std::cout << error_message;
            continue;
        }
        return static_cast<T>(value);
    }
}

void PrintNotEnoughArea(const hamurabi::NotEnoughArea error) {
    std::cout << "HAMURABI: THINK AGAIN. YOU OWN ONLY "
              << error.Area() << " ACRES. NOW THEN,\n";
}

void PrintNotEnoughGrain(const hamurabi::NotEnoughGrain error) {
    std::cout << "HAMURABI: THINK AGAIN. YOU HAVE ONLY\n"
              << error.Grain() << "BUSHELS OF GRAIN. NOW THEN,\n";
}

void PrintNotEnoughPeople(const hamurabi::NotEnoughPeople error) {
    std::cout << "HAMURABI: THINK AGAIN. YOU HAVE ONLY "
              << error.Population() << " PEOPLE TO TEND THE FIELDS! NOW THEN,\n";
}

void PrintGameOver(const hamurabi::GameOver game_over) {
    std::cout << "YOU STARVED " << game_over.DeadFromHunger() << " PEOPLE IN ONE YEAR!!!\n"
              << "DUE TO THIS EXTREME MISMANAGEMENT YOU HAVE NOT ONLY\n"
                 "BEEN IMPEACHED AND THROWN OUT OF OFFICE BUT YOU HAVE\n"
                 "ALSO BEEN DECLARED NATIONAL FINK!!!!\n";
}

template<class... Ts>
struct overloaded : Ts ... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<class T>
hamurabi::AreaToBuy ReceiveAreaToBuy(const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToBuyResult> result;
    auto try_again = true;
    while (try_again) {
        input = ReceiveUnsigned<decltype(input)>("HOW MANY ACRES DO YOU WISH TO BUY? ");
        result = hamurabi::AreaToBuy::New(input, game);
        std::visit(overloaded{
            [&try_again](hamurabi::AreaToBuy) { try_again = false; },
            [](hamurabi::NotEnoughGrain error) { PrintNotEnoughGrain(error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToBuy>(result.value());
}

template<class T>
hamurabi::AreaToSell ReceiveAreaToSell(const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToSellResult> result;
    auto try_again = true;
    while (try_again) {
        input = ReceiveUnsigned<decltype(input)>("HOW MANY ACRES DO YOU WISH TO SELL? ");
        result = hamurabi::AreaToSell::New(input, game);
        std::visit(overloaded{
            [&try_again](hamurabi::AreaToSell) { try_again = false; },
            [](hamurabi::NotEnoughArea error) { PrintNotEnoughArea(error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToSell>(result.value());
}

template<class T>
hamurabi::GrainToFeed ReceiveGrainToFeed(const hamurabi::Game<T> &game) {
    hamurabi::Bushels input;
    std::optional<hamurabi::GrainToFeedResult> result;
    auto try_again = true;
    while (try_again) {
        input = ReceiveUnsigned<decltype(input)>("HOW MANY BUSHELS DO YOU WISH TO FEED YOUR PEOPLE? ");
        result = hamurabi::GrainToFeed::New(input, game);
        std::visit(overloaded{
            [&try_again](hamurabi::GrainToFeed) { try_again = false; },
            [](hamurabi::NotEnoughGrain error) { PrintNotEnoughGrain(error); },
        }, result.value());
    }
    return std::get<hamurabi::GrainToFeed>(result.value());
}

template<class T>
hamurabi::AreaToPlant ReceiveAreaToPlant(const hamurabi::Game<T> &game) {
    hamurabi::Acres input;
    std::optional<hamurabi::AreaToPlantResult> result;
    auto try_again = true;
    while (try_again) {
        input = ReceiveUnsigned<decltype(input)>("HOW MANY ACRES DO YOU WISH TO PLANT WITH SEED? ");
        result = hamurabi::AreaToPlant::New(input, game);
        std::visit(overloaded{
            [&try_again](hamurabi::AreaToPlant) { try_again = false; },
            [](hamurabi::NotEnoughArea error) { PrintNotEnoughArea(error); },
            [](hamurabi::NotEnoughGrain error) { PrintNotEnoughGrain(error); },
            [](hamurabi::NotEnoughPeople error) { PrintNotEnoughPeople(error); },
        }, result.value());
    }
    return std::get<hamurabi::AreaToPlant>(result.value());
}

template<class T>
hamurabi::RoundInput ReceiveRoundInput(const hamurabi::Game<T> &game) {
    const auto area_to_buy = ReceiveAreaToBuy(game);
    const auto area_to_sell = ReceiveAreaToSell(game);
    const auto grain_to_feed = ReceiveGrainToFeed(game);
    const auto area_to_plant = ReceiveAreaToPlant(game);
    std::cout << "\n";

    return hamurabi::RoundInput{
        .area_to_buy = area_to_buy,
        .area_to_sell = area_to_sell,
        .grain_to_feed = grain_to_feed,
        .area_to_plant = area_to_plant,
    };
}

template<class T>
void PrintGameState(const hamurabi::Game<T> &game) {
    std::cout << "HAMURABI:  I BEG TO REPORT TO YOU,\n"
                 "IN YEAR " << game.CurrentRound() << ",";
    if (game.DeadFromHunger() > 0) {
        std::cout << " " << game.DeadFromHunger() << " PEOPLE STARVED,";
    }
    if (game.Arrived() > 0) {
        std::cout << " " << game.Arrived() << " PEOPLE CAME TO THE CITY,";
    }
    std::cout << "\n";

    if (game.IsPlague()) {
        std::cout << "A HORRIBLE PLAGUE STRUCK!  HALF THE PEOPLE DIED.\n";
    }
    std::cout << "POPULATION IS NOW " << game.Population() << "\n"
              << "THE CITY NOW OWNS " << game.Area() << " ACRES\n"
              << "YOU HARVESTED " << game.GrainFromAcre() << " BUSHELS PER ACRE.\n";
    if (game.GrainEatenByRats() > 0) {
        std::cout << "THE RATS ATE " << game.GrainEatenByRats() << " BUSHELS.\n";
    }
    std::cout << "YOU NOW HAVE " << game.Grain() << " BUSHELS IN STORE.\n"
              << "LAND IS TRADING AT " << game.AcrePrice() << " BUSHELS PER ACRE.\n";
}

void PrintEndStatistics(const hamurabi::EndStatistics statistics) {
    std::cout << "IN YOUR 10-YEAR TERM OF OFFICE, " << statistics.AverageDeadFromHungerPercent() << " PERCENT OF THE\n"
              << "POPULATION STARVED PER YEAR ON THE AVERAGE, I.E. A TOTAL OF\n"
              << statistics.DeadFromHunger() << " PEOPLE DIED!!\n"
              << "YOU STARTED WITH 10 ACRES PER PERSON AND ENDED WITH\n"
              << statistics.AreaByPerson() << " ACRES PER PERSON\n";

    using Rank = hamurabi::EndStatistics::Rank;
    switch (statistics.CalculateRank()) {
        case Rank::D: {
            std::cout << "THE PEOPLE (REMAINING) FIND YOU AN UNPLEASANT RULER, AND,\n"
                         "FRANKLY, HATE YOUR GUTS!\n";
            break;
        }
        case Rank::C: {
            std::cout << "YOUR HEAVY-HANDED PERFORMANCE SMACKS OF NERO AND IVAN IV.\n";
            break;
        }
        case Rank::B: {
            std::cout << "YOUR PERFORMANCE COULD HAVE BEEN SOMEWHAT BETTER, BUT\n"
                         "REALLY WASN'T TOO BAD AT ALL. PEOPLE\n"
                         "DEARLY LIKE TO SEE YOU ASSASSINATED BUT WE ALL HAVE OUR\n"
                         "TRIVIAL PROBLEMS.\n";
            break;
        }
        case Rank::A: {
            std::cout << "A FANTASTIC PERFORMANCE!!! CHARLEMAGNE, DISRAELI, AND\n"
                         "JEFFERSON COMBINED COULD NOT HAVE DONE BETTER!\n";
            break;
        }
    }
}

void PrintGoodbye() {
    std::cout << "\nSO LONG FOR NOW.\n";
}

int main() {
    std::random_device random_device;
    std::mt19937 generator{random_device()};
    hamurabi::Game game{generator};

    PrintGreetings();
    PrintGameState(game);

    auto can_play = true;
    while (can_play) {
        hamurabi::RoundInput input = ReceiveRoundInput(game);
        hamurabi::RoundResult result = game.PlayRound(input);
        std::visit(overloaded{
            [&can_play](hamurabi::GameOver game_over) {
                PrintGameOver(game_over);
                can_play = false;
            },
            [&can_play](hamurabi::EndStatistics statistics) {
                PrintEndStatistics(statistics);
                can_play = false;
            },
            [&game = std::as_const(game)](hamurabi::Continue) {
                PrintGameState(game);
            },
        }, result);
    }

    PrintGoodbye();
    return 0;
}
