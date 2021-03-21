#pragma once

#include <cstdint>
#include <ostream>

enum Side
{
    Buy = 0,
    Sell = 1
};

enum Action
{
    Add = 0,
    Remove = 1
};

struct Order
{
    uint64_t id_;
    int      price_;
    int      qty_;
    Side     side_;
};

inline bool operator==(Order const& order1, Order const& order2)
{
    return order1.id_ == order2.id_
           and order1.price_ == order2.price_
           and order1.side_ == order2.side_
           and order1.qty_ == order2.qty_;
}

inline std::ostream& operator<<(std::ostream& stream, Order const& order)
{
    stream << "id: " << order.id_ << ",";
    stream << "qty: " << order.qty_ << ",";
    stream << "price: " << order.price_ << ",";
    stream << ((order.side_ == Side::Buy)?"Buy":"Sell");
    return stream;
}
