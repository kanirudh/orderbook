add_library(orderbook_orderbook
    STATIC
    OrderBook.cpp
    MarketDataParser.cpp
    )
add_library(orderbook::orderbook ALIAS orderbook_orderbook)
