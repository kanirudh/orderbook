#include "MarketDataParser.h"
#include "LineFormatter.h"


int main(int argc, char* argv[])
{
    MarketDataParser<CsvLineFormatter> mdp(argv[1]);

    auto [order, valid] = mdp.GetNextOrder();
    while (valid)
    {

        std::tie(order, valid) = mdp.GetNextOrder();
    }
    return 0;
}
