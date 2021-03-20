#include "OrderBook.h"

#include <iostream>
#include <algorithm>

void OrderBook::HandleOrder(Order order, Action action)
{
    if (action == Action::Remove)
    {
        removeFromBook(order);
        return;
    }

    checkForCross(order);

    if (order.qty_ != 0)
    {
        addToBook(order);
    }
}

template <typename Comp>
void OrderBook::checkForCrossImpl(Order& order, Book& book, Comp comp)
{
    auto levelItr = book.begin();
    while (levelItr != book.end() and comp(levelItr->first, order.price_))
    {
        auto orderItr = levelItr->second.begin();
        while (orderItr != levelItr->second.end() and order.qty_ != 0)
        {
            if (orderItr->qty_ > order.qty_) {
                orderItr->qty_ -= order.qty_;
                order.qty_ = 0;
                break;
            }
            order.qty_ -= orderItr->qty_;
            ++orderItr;
        }
        levelItr->second.erase(levelItr->second.begin(), orderItr);
        if (levelItr->second.empty())
            // If all orders on this level are zero move on to next Level
            ++levelItr;
        else
            break;
    }
    book.erase(book.begin(), levelItr);
}

void OrderBook::checkForCross(Order& order)
{
    if (order.side_ == Side::Buy)
    {
        checkForCrossImpl(order, sell_orders_, [](int first, int second) {
            return first <= second;
        });
    }
    else
    {
        checkForCrossImpl(order, buy_orders_, [](int first, int second) {
            return first >= second;
        });
    }
}

template <typename Comparator>
void OrderBook::addToBookImpl(Order const& order, Book& book, Comparator cmp)
{
    auto itr = std::lower_bound(book.begin(), book.end(), order, cmp);
    if (itr == book.end() or itr->first != order.price_)
    {
        // The level doesn't exist in the OrderBook already insert it
        book.emplace(itr, order.price_, std::vector({order}));
    }
    else
    {
        itr->second.emplace_back(order);
    }
}

void OrderBook::addToBook(Order const& order)
{
    if (Side::Buy == order.side_)
    {
        auto cmp = [](Level const& first, Order const& second) {
            if (first.first > second.price_)
            {
                return true;
            }
            return false;
        };
        addToBookImpl(order, buy_orders_, cmp);
    }
    else
    {
        auto cmp = [](Level const& first, Order const& second) {
            if (first.first < second.price_)
            {
                return true;
            }
            return false;
        };
        addToBookImpl(order, sell_orders_, cmp);
    }
}

template <typename Comparator>
void OrderBook::removeFromBookImpl(Order const& order, Book& book, Comparator cmp)
{
    auto itr = std::lower_bound(book.begin(), book.end(), order, cmp);
    if (itr != book.end() and itr->first == order.price_)
    {
        auto& levelOrders = itr->second;
        levelOrders.erase(std::remove_if(levelOrders.begin(), levelOrders.end(),
                    [&order](Order const& list_order) {
                        return list_order.id_  == order.id_;
                    }),
                    levelOrders.end());

        if (levelOrders.empty())
            book.erase(itr);
    }
}

void OrderBook::removeFromBook(Order const& order)
{
    if (Side::Buy == order.side_)
    {
        auto cmp = [](Level const& first, Order const& second) {
            if (first.first > second.price_)
            {
                return true;
            }
            return false;
        };
        removeFromBookImpl(order, buy_orders_, cmp);
    }
    else
    {
        auto cmp = [](Level const& first, Order const& second) {
            if (first.first < second.price_)
            {
                return true;
            }
            return false;
        };
        removeFromBookImpl(order, sell_orders_, cmp);
    }
}

void OrderBook::Display() const
{
    std::cout << " Order Book " << std::endl;
    std::cout << "Price, Quantity, UID, Buy/Sell" << std::endl;

    for (int level = sell_orders_.size() - 1; level >= 0; level--)
    {
        auto const& orderlist = sell_orders_[level].second;
        for (int i = orderlist.size()-1;i >= 0; i--) {
            auto const& order = orderlist[i];
            std::cout << order.price_ << ",";
            std::cout << order.qty_ << ",";
            std::cout << order.id_ << ",";
            std::cout << ((order.side_ == Side::Buy)?"Buy":"Sell");
            std::cout << std::endl;
        }
    }

    for (auto const& [price, orderlist] : buy_orders_)
    {
        for (auto const& order : orderlist) {
            std::cout << order.price_ << ",";
            std::cout << order.qty_ << ",";
            std::cout << order.id_ << ",";
            std::cout << ((order.side_ == Side::Buy)?"Buy":"Sell");
            std::cout << std::endl;
        }
    }


}
