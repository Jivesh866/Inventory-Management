#pragma once

#include <iostream>
#include <string>
#include "customer.hpp"
#include <sqlite3.h>
#include <optional>

void initialize(sqlite3 *db);
void insertCustomer(sqlite3* db, const std::string& name, const std::string& email, long long phone, int age,const std::string& password);
void fetchCustomers(sqlite3 *db);
int getItemPriceById(int item_id);
int getItemPriceBySku(std::string& sku);
bool checkEmail(sqlite3* db, const std::string& email);
std::optional<std::string> getName(sqlite3* db, const std::string& email);
std::optional<int> getId(sqlite3* db,const std::string& email);
bool checkPassword(sqlite3* db, const std::string& email, const std::string& password);
int getItemQuantity(int item_id);
std::optional<Customer> fetchCustomerById(sqlite3 *db, int id);
std::optional<Customer> fetchCustomerByEmail(sqlite3 *db,const std::string &email);
bool checkPhone(sqlite3 *db, long long phone);
void deleteUser(sqlite3 *db,const std::string& email);