project(server LANGUAGES CXX)

add_library(server_lib STATIC
    src/arg_parser.cpp
    src/config_reader.cpp
    src/pattern_matcher.cpp
    src/server.cpp
    src/statistic_manager.cpp
)

target_include_directories(server_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server_lib PUBLIC
    common
    nlohmann_json::nlohmann_json
    pthread
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE server_lib)