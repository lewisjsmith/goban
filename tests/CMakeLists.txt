add_executable(run_tests test_main.cpp)

target_link_libraries(run_tests PRIVATE gtest_main game_lib)

include(GoogleTest)
gtest_discover_tests(run_tests)