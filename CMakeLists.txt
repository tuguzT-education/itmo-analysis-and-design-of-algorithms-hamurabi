cmake_minimum_required(VERSION 3.26)
project(Hammurabi)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hammurabi
        src/main.cpp
        src/Hammurabi/City.h
        src/Hammurabi/PlayerInput.h
        src/Hammurabi/PlayerOutput.h
        src/Hammurabi/Resources.h
        src/Hammurabi/Game.h src/Hammurabi/Game.cpp)
