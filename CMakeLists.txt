project(go-game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(prog src/main.cpp)

target_include_directories(prog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)