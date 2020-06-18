cmake_minimum_required(VERSION 2.8)

include_directories(include)

add_executable(main src/main.cpp)
add_library(pipes_lib src/pipes.cc)

target_link_libraries(
    main pipes_lib
)
