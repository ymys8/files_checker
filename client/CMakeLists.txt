project(client LANGUAGES CXX)

set(PROJECT_SOURCES
    src/main.cpp
    src/client.cpp
    src/arg_parser.cpp
    src/file_reader.cpp

    include/client.h
    include/arg_parser.h
    include/file_reader.h
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)