cmake_minimum_required(VERSION 3.25)
project(tramway-tests)

set(CMAKE_CXX_STANDARD 17)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

include_directories(../src)
set(SOURCE_FILES
        ../src/GameEngine/Card.cpp
        ../src/GameEngine/Deck.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/CombinedStrategy.cpp
        ../src/GameEngine/GameEngine.cpp
        ../src/GameEngine/GameEngine.h
        ../src/GameState.cpp
        ../src/GameState.h
        ../src/GameEngine/Player/Player.h
        ../src/GameEngine/Player/Player.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundStrategy.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundStrategy.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundStrategy.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundStrategy.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundStrategy.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundFullRandom.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundFullRandom.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundOptimal.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundOptimal.h
        ../src/GameEngine/Hand.cpp
        ../src/GameEngine/Hand.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundNaive.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundNaive.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundNaive.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundNaive.h
        ../src/GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundNaive.cpp
        ../src/GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundNaive.h
        ../src/Visualizer/Visualizer.cpp
        ../src/GameRules.h
        ../src/Reporter/GameReport.cpp
        ../src/Reporter/GameReport.h
        ../src/Reporter/Reporter.cpp
        ../src/Reporter/Reporter.h
)

set(TEST_SOURCES
        CardTest.cpp DeckTest.cpp HandTest.cpp FirstRoundOptimalTest.cpp NaiveStrategyTest.cpp)

add_executable(Tests_run ${SOURCE_FILES} ${TEST_SOURCES})

gtest_discover_tests(Tests_run)
target_link_libraries(Tests_run gtest gtest_main)