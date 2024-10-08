cmake_minimum_required(VERSION 3.25)
project(tramway-app)

set(CMAKE_CXX_STANDARD 17)

add_executable( tramway_app
        main.cpp
        GameEngine/Deck.cpp
        GameEngine/Deck.h
        GameEngine/Card.h
        Exceptions/EmptyDeckException.h
        GameEngine/Player/Player.cpp
        GameEngine/Player/Player.h
        GameState.cpp
        GameState.h
        GameEngine/Card.cpp
        GameEngine/Player/Strategy/CombinedStrategy/CombinedStrategy.h
        GameEngine/Player/Strategy/CombinedStrategy/CombinedStrategy.cpp
        GameEngine/GameEngine.cpp
        GameEngine/GameEngine.h
        GameState.h
        GameEngine/Player/Strategy/IStrategy.h
        GameEngine/Decision.h
        GameEngine/Decision.h
        GameEngine/Player/Strategy/IStrategy.h
        GameEngine/Player/Strategy/IStrategy.h GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundStrategy.cpp GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundStrategy.h GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundStrategy.cpp GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundStrategy.h GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundStrategy.h Exceptions/InvalidGameStateException.h GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundFullRandom.cpp GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundFullRandom.h GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundOptimal.cpp GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundOptimal.h GameEngine/Hand.cpp GameEngine/Hand.h Exceptions/InvalidPlayerDecisionException.h GameEngine/Player/Strategy/InteractiveStrategy.cpp GameEngine/Player/Strategy/InteractiveStrategy.h GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundNaive.cpp GameEngine/Player/Strategy/CombinedStrategy/SecondRound/SecondRoundNaive.h GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundNaive.cpp GameEngine/Player/Strategy/CombinedStrategy/FirstRound/FirstRoundNaive.h GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundNaive.cpp GameEngine/Player/Strategy/CombinedStrategy/ThirdRound/ThirdRoundNaive.h Visualizer/Visualizer.cpp Visualizer/Visualizer.h GameRules.h Reporter/GameReport.cpp Reporter/GameReport.h Reporter/Reporter.cpp Reporter/Reporter.h
)