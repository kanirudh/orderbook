add_executable(orderbook_test
    main.cpp
    MarketDataParser.t.cpp
    OrderBook.t.cpp
    )
target_link_libraries(orderbook_test
    orderbook::orderbook
    gtest
    pthread
    )
# This should ideally come from the library, look at it later
target_include_directories(orderbook_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    )

gtest_discover_tests(orderbook_test)

