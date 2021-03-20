#include "src/MarketDataParser.h"
#include "src/LineFormatter.h"
#include "src/OrderBook.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./a.out <market data file>" << std::endl;
        return -1;
    }

    MarketDataParser<CsvLineFormatter> mdp(argv[1]);
    OrderBook ob;

    while (auto data = mdp.GetNextOrder())
    {
        std::cout << data->first << " " << data->second << std::endl;
        ob.HandleOrder(data->first, data->second);
        ob.Display();
    }
    return 0;
}
