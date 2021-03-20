#pragma once
#include "Order.h"

#include <string>
#include <string_view>
#include <charconv>

//TODO remove this
#include <iostream>

/*
 * Following input format
 * A,100000,S,1,1075
 * Addition/Deletion,ID,Side,Quantity,Price
 */
class CsvLineFormatter
{
public:
    std::pair<Order, Action> operator()(std::string_view line) const
    {
        std::pair<Order, Action> result;
        auto& order = result.first;

        int start = 0, end = 0;
        end = line.find_first_of(",");
        result.second  = ((line.substr(start, end - start) == "A")?Action::Add:Action::Remove);
        start = end + 1;

        // Parse id
        end = line.find_first_of(",", start);
        std::from_chars(line.data() + start, line.data() + end, order.id_);
        start = end + 1;

        // Parse Side
        end = line.find_first_of(",", start);
        order.side_ = ((line.substr(start, end - start) == "S")?Side::Sell:Side::Buy);
        start = end + 1;

        // Parser Quantity
        end = line.find_first_of(",", start);
        std::from_chars(line.data() + start, line.data() + end, order.qty_);
        start = end + 1;

        // Parse Price
        end = line.size();
        std::from_chars(line.data() + start, line.data() + end, order.price_);

        return result;
    }
};
