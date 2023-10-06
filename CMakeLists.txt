cmake_minimum_required(VERSION 3.26)
project(Hamurabi)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hamurabi src/main.cpp
        src/Hamurabi/Resources.hpp
        src/Hamurabi/Continue.hpp
        src/Hamurabi/GameEnd.hpp
        src/Hamurabi/Detail.hpp src/Hamurabi/Detail.inl
        src/Hamurabi/Game.fwd src/Hamurabi/Game.hpp src/Hamurabi/Game.inl
        src/Hamurabi/NotEnoughArea.hpp src/Hamurabi/NotEnoughArea.inl
        src/Hamurabi/NotEnoughGrain.hpp src/Hamurabi/NotEnoughGrain.inl
        src/Hamurabi/NotEnoughPeople.hpp src/Hamurabi/NotEnoughPeople.inl
        src/Hamurabi/AreaToBuy.hpp src/Hamurabi/AreaToBuy.inl
        src/Hamurabi/AreaToSell.hpp src/Hamurabi/AreaToSell.inl
        src/Hamurabi/GrainToFeed.hpp src/Hamurabi/GrainToFeed.inl
        src/Hamurabi/AreaToPlant.hpp src/Hamurabi/AreaToPlant.inl
        src/Hamurabi/RoundInput.hpp src/Hamurabi/RoundInput.inl
        src/Hamurabi/GameOver.hpp src/Hamurabi/GameOver.inl
        src/Hamurabi/Statistics.hpp src/Hamurabi/Statistics.inl
        src/Play/Detail.hpp src/Play/Detail.inl
        src/Play/Hamurabi.hpp src/Play/Hamurabi.inl)
