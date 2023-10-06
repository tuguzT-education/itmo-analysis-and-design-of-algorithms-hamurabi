#ifndef PLAY_HAMURABI_INL
#define PLAY_HAMURABI_INL

namespace play {

template<class T>
void Hamurabi(std::istream &istream, std::ostream &ostream, hamurabi::Game<T> game) {
    detail::InsertGreetings(ostream);
    detail::InsertGameState(ostream, game);

    bool can_play = true;
    while (can_play) {
        hamurabi::RoundInput input = detail::ExtractRoundInput(istream, ostream, game);
        hamurabi::RoundResult result = game.PlayRound(input);
        std::visit(detail::overloaded{
            [&ostream, &can_play](hamurabi::GameOver game_over) {
                detail::InsertGameOver(ostream, game_over);
                can_play = false;
            },
            [&ostream, &game = std::as_const(game)](hamurabi::Continue) {
                detail::InsertGameState(ostream, game);
            },
            [&ostream, &game = std::as_const(game), &can_play](hamurabi::GameEnd) {
                const auto statistics = game.Statistics().value();
                detail::InsertGameState(ostream, game);
                detail::InsertGameStatistics(ostream, statistics);
                can_play = false;
            },
        }, result);
    }

    detail::InsertGoodbye(ostream);
}

}

#endif //PLAY_HAMURABI_INL
