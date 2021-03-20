#pragma once

#include "Order.h"
#include "gtest/gtest_prod.h"

#include <vector>
#include <unordered_map>

/*
 * \brief Saves the currently open orders, in a priority queue
 * It saves both sides of book in a a vector
 *
 * 1. If any order crosses the book, then we just have to do a simple iteration on open orders
 * 2. Otherwise, to find the correct place in the order book, we have to look
 *      pricesorder.action_
 */
class OrderBook
{
    // This is not a problem because we are not allowing only known test classes access
    FRIEND_TEST(OrderBookTest, RemoveOrder);
    FRIEND_TEST(OrderBookTest, AddOrders);
    FRIEND_TEST(OrderBookTest, UnCrossedBook);

    using OrderList = std::vector<Order>;
    using Level = std::pair<int, OrderList>;
    using Book = std::vector<Level>;

public:

    void HandleOrder(Order order, Action action);
    void Display() const;

protected:
    void checkForCross(Order& order);
    void addToBook(Order const& order);
    void removeFromBook(Order const& order);

private:

    template <typename Comp>
    void checkForCrossImpl(Order& order, Book& book, Comp comp);

    template <typename Comparator>
    void addToBookImpl(Order const& order, Book& book, Comparator cmp);

    template <typename Comparator>
    void removeFromBookImpl(Order const& order, Book& book, Comparator cmp);

    Book buy_orders_;
    Book sell_orders_;
};
