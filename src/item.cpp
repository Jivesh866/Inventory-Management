#include <iostream>
#include "item.hpp"
#include <format>

Item::Item(string& type, string& name, int id)
{
    this->type = type;
    this->name = name;
    this->id = id;
}

Item::Item(string& type, string& name, int id, int quantity)
{
    this->type = type;
    this->name = name;
    this->id = id;
    this->quantity = quantity;
}

void Item::print()
{
    cout << format("Name: {}, Type: {}, ID: {}, Quantity:", name, type, id, quantity);
}

void Item::updateQuantity(int new_stock){
    this->quantity=new_stock;
}

bool Item::isLowStock() const{
    if(quantity<reorder_threshold){
        return true;
    }else{
        return false;
    }
}