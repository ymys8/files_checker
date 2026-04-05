project(client LANGUAGES CXX)

add_library(client_lib STATIC
    src/client.cpp
    src/arg_parser.cpp
    src/file_reader.cpp
)

target_include_directories(client_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE client_lib)