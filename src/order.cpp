#include "order.hpp"
#include <iostream>
#include "database.hpp"

using namespace std;

int Order::calculatePrice()
{
    int item_id, quantity, price, price_sum;

    for (int i = 0; i < items.size(); i++)
    {
        item_id = items[i].first;
        quantity = items[i].second;
        price = getItemPriceById(item_id);
        price_sum = price * quantity;
    }

    return price_sum;
}

void Order::addItem(int item_id, int quantity)
{
    items.emplace_back(item_id, quantity);
}