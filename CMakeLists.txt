cmake_minimum_required(VERSION 3.10)

# set the project name
project(OrderBook)

set(CMAKE_CXX_STANDARD 17)

include(GoogleTest)

# add the executable
add_executable(OrderBookSample
    main.cpp
    )
target_link_libraries(OrderBookSample
    PRIVATE
    orderbook::orderbook
    )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(tests)
