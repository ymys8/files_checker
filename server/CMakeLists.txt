project(server LANGUAGES CXX)

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

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    common
    nlohmann_json::nlohmann_json
)