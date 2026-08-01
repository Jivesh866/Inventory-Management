#pragma once

#include <iostream>
#include <string>
#include <sqlite3.h>

void initialize(sqlite3 *db);
void insertCustomer(sqlite3* db, const std::string& name, const std::string& email, double phone, int age,const std::string& password);
void fetchCustomers(sqlite3 *db);
int getItemPriceById(int item_id);
int getItemPriceBySku(int sku);
bool checkEmail(sqlite3* db, const std::string& email);
int getItemQuantity(int item_id);