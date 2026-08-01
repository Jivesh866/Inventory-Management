#pragma once

#include <iostream>
using namespace std;

class Item
{
private:
    string type;
    string name;
    int id;
    string sku;
    int price;
    int reorder_threshold = 5;
    int quantity = 0;

public:
    Item(string& type, string& name, int id);
    Item(string& type, string& name, int id, int quantity);
    void print();
    void updateQuantity(int new_stock);
    bool isLowStock() const;

    void setPrice(int new_price)
    {
        price=new_price;
    }
    string getName() const
    {
        return this->name;
    }
    string getSku() const
    {
        return this->sku;
    }
    string getType() const
    {
        return this->type;
    }
    int getId() const
    {
        return this->id;
    }
    int getPrice() const
    {
        return this->price;
    }
    int getReorderThreshold() const
    {
        return this->reorder_threshold;
    }
    int getQuantity() const
    {
        return this->quantity;
    }
};