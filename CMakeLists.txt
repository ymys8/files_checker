cmake_minimum_required(VERSION 3.24)
project(hometask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(util)

enable_testing()
add_subdirectory(tests)