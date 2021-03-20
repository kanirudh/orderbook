#include "Order.h"

#include <fstream>
#include <string>
#include <array>
#include <optional>

/*
 * TODO can be tempaltized on the input, and order
 */
template <typename LineFormatter>
class MarketDataParser
{
public:
    explicit MarketDataParser(std::string filename)
        : filename_(std::move(filename))
    {
        fs_.open(filename_);
        if (not fs_.is_open())
        {
            throw std::runtime_error("Failed to open the file");
        }
    }

    std::optional<std::pair<Order, Action>> GetNextOrder()
    {
        std::string line;
        if (std::getline(fs_, line))
        {
            return formatter_(line);
        }
        return std::nullopt;
    }

    ~MarketDataParser()
    {
        fs_.close();
    }

private:
    LineFormatter formatter_;
    std::string filename_;
    std::fstream fs_;
};
