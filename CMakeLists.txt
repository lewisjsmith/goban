project(go-game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib
    src/board.cpp
)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(prog src/main.cpp)

target_link_libraries(prog PRIVATE lib)