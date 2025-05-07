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

    add_executable(RegexToDFA main.cpp
            DFA.cpp
            DFA.h
            NFA.cpp
            NFA.h
            NFA_LAMBDA.cpp
            NFA_LAMBDA.hpp)


    target_link_libraries(RegexToDFA PRIVATE nlohmann_json::nlohmann_json)