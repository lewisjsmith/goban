project(go_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(game_lib
    src/board.cpp
)

target_include_directories(game_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(prog src/main.cpp)
target_link_libraries(prog PRIVATE game_lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

add_subdirectory(tests)