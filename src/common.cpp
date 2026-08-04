#include <cctype>
#include <algorithm>
#include <iostream>
#include "common.hpp"

void removeWS(string &text)
{
    auto [first, last] = std::ranges::remove_if(text, [](unsigned char c)
                                                { return std::isspace(c); });
    text.erase(first, last);
}