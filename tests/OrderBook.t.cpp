#include <gtest/gtest.h>

#include <src/OrderBook.h>

class OrderBookTest : public ::testing::Test
{
protected:
    OrderBook ob;
};

/*
 * There are two things to check for here,
 * the ordering for buy and sell orders
 * and ther ordering for timestamp values
 */
TEST_F(OrderBookTest, AddOrders)
{
    ob.addToBook({1, 95, 10, Side::Buy});
    ob.addToBook({2, 93, 10, Side::Buy});
    ob.addToBook({3, 95, 42, Side::Buy});
    ob.addToBook({4, 97, 42, Side::Buy});
    ob.addToBook({5, 105, 42, Side::Sell});
    ob.addToBook({6, 103, 42, Side::Sell});

    ASSERT_EQ(ob.buy_orders_.size(), 3);
    EXPECT_EQ(ob.buy_orders_[0].first, 97);
    EXPECT_EQ(ob.buy_orders_[0].second[0].id_, 4);
    EXPECT_EQ(ob.buy_orders_[1].first, 95);
    EXPECT_EQ(ob.buy_orders_[1].second[0].id_, 1);
    EXPECT_EQ(ob.buy_orders_[1].second[1].id_, 3);
    EXPECT_EQ(ob.buy_orders_[2].first, 93);
    EXPECT_EQ(ob.buy_orders_[2].second[0].id_, 2);

    ASSERT_EQ(ob.sell_orders_.size(), 2);
    EXPECT_EQ(ob.sell_orders_[0].first, 103);
    EXPECT_EQ(ob.sell_orders_[0].second[0].id_, 6);
    EXPECT_EQ(ob.sell_orders_[1].first, 105);
    EXPECT_EQ(ob.sell_orders_[1].second[0].id_, 5);
}

TEST_F(OrderBookTest, RemoveOrder)
{
    ob.addToBook({1, 95, 10, Side::Buy});
    ob.addToBook({2, 93, 10, Side::Buy});
    ob.addToBook({3, 95, 42, Side::Buy});
    ob.addToBook({4, 97, 42, Side::Buy});
    ob.addToBook({5, 105, 42, Side::Sell});

    ob.removeFromBook({1, 95, 10, Side::Buy});

    ASSERT_EQ(ob.buy_orders_.size(), 3);
    ASSERT_EQ(ob.buy_orders_[1].second.size(), 1);
    ASSERT_EQ(ob.buy_orders_[1].second[0].id_, 3);

    // Remove an entire level from the book
    ob.removeFromBook({3, 95, 42, Side::Buy});
    ASSERT_EQ(ob.buy_orders_.size(), 2);
    ASSERT_EQ(ob.buy_orders_[0].first, 97);
    ASSERT_EQ(ob.buy_orders_[1].first, 93);


    // Remove a non-existent Order, nothing should crash
    ob.removeFromBook({199934, 132,234, Side::Buy});
}

TEST_F(OrderBookTest, UnCrossedBook)
{
    ob.HandleOrder({1, 95, 10, Side::Buy}, Action::Add);
    ob.HandleOrder({2, 105, 10, Side::Sell}, Action::Add);
    ob.HandleOrder({3, 97, 10, Side::Buy}, Action::Add);
    ob.HandleOrder({4, 106, 11, Side::Sell}, Action::Add);
    ob.HandleOrder({5, 105, 7, Side::Sell}, Action::Add);

    // Send an aggresive order
    ob.HandleOrder({6, 105, 30, Side::Buy}, Action::Add);

    ASSERT_EQ(ob.sell_orders_.size(), 1);
    EXPECT_EQ(ob.sell_orders_[0].first, 106);
    EXPECT_EQ(ob.sell_orders_[0].second[0].id_, 4);

    ASSERT_EQ(ob.buy_orders_.size(), 3);
    EXPECT_EQ(ob.buy_orders_[0].first, 105);
    EXPECT_EQ(ob.buy_orders_[0].second[0].qty_, 13);
    EXPECT_EQ(ob.buy_orders_[0].second[0].id_, 6);
    EXPECT_EQ(ob.buy_orders_[1].first, 97);
    EXPECT_EQ(ob.buy_orders_[1].second[0].id_, 3);
    EXPECT_EQ(ob.buy_orders_[2].first, 95);
    EXPECT_EQ(ob.buy_orders_[2].second[0].id_, 1);
}
