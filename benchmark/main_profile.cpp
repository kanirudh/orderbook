#include "src/MarketDataParser.h"
#include "src/LineFormatter.h"
#include "src/OrderBook.h"

#include <iostream>
#include <chrono>
#include <random>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./a.out number_of_iterations" << std::endl;
        return -1;
    }

    OrderBook ob;

    std::random_device rd{};
    std::mt19937 gen1{rd()};
    std::mt19937 gen2{rd()};

    // This is a realistic assumption most trading will be centered around yesterday
    // close prices and will have trading cutoffs
    std::normal_distribution<> price_dist{10000, 50};

    std::uniform_int_distribution<int> quantity_dist(1, 1e5);

    int N = std::stoi(argv[1]);


    // Run a million order simulation
    for (int i = 0; i < N; i++) {
        int price = price_dist(gen1);
        int quantity = quantity_dist(gen2);
        Order o{i, price, quantity, (i&1)?Side::Buy:Side::Sell};
        ob.HandleOrder(o, Action::Add);
        /*std::cout << o << std::endl;
        ob.Display();*/
    }
    return 0;
}
