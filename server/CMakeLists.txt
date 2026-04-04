cmake_minimum_required(VERSION 3.24)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

set(PROJECT_SOURCES
    src/main.cpp
    src/arg_parser.cpp
    src/config_reader.cpp
    src/pattern_matcher.cpp
    src/server.cpp
    src/statistic_manager.cpp

    include/arg_parser.h
    include/config_reader.h
    include/pattern_matcher.h
    include/server.h
    include/statistic_manager.h
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)