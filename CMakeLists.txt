    cmake_minimum_required(VERSION 3.30)
    project(RegexToDFA)

    set(CMAKE_CXX_STANDARD 20)
    include(FetchContent)
    FetchContent_Declare(
            json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.11.2 # or latest tag
    )
    FetchContent_MakeAvailable(json)

    add_executable(RegexToDFA Source/main.cpp
            Source/DFA.cpp
            Source/DFA.h
            Source/NFA.cpp
            Source/NFA.h
            Source/NFA_LAMBDA.cpp
            Source/NFA_LAMBDA.hpp
            Source/CFG.cpp
            Source/CFG.hpp
            Source/RandomNumberGenerator.cpp
            Source/RandomNumberGenerator.hpp)


    target_link_libraries(RegexToDFA PRIVATE nlohmann_json::nlohmann_json)