cmake_minimum_required(VERSION 3.26)
project(Hamurabi)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hamurabi src/main.cpp
        src/Hamurabi/Resources.h
        src/Hamurabi/Detail.h
        src/Hamurabi/Game_decl.h src/Hamurabi/Game.h)
