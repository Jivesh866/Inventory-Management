#pragma once

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class Order{
    private:
    int order_id;
    vector<pair<int,int>> items; //[Item id:Quanity]
    int customer_id;
    string status;
    public:

    int calculatePrice();
    void addItem(int item_id,int quantity);
    int getOrderId(){
        return order_id;
    }
};