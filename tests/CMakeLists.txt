include(GoogleTest)

add_executable(test_client
    test_client.cpp
)
target_link_libraries(test_client PRIVATE
    client_lib
    GTest::gtest_main
)
gtest_discover_tests(test_client)

add_executable(test_server
    test_server.cpp
)
target_link_libraries(test_server PRIVATE
    server_lib
    GTest::gtest_main
)
gtest_discover_tests(test_server)
