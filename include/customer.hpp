#pragma once

#include <iostream>
#include <sqlite3.h>

using namespace std;

class Customer
{
private:
    int id;
    string name;
    string email;
    long long phone;
    int age;

public:
    Customer(int id, const string &name, const string &email, long long phone, int age);
    Customer() {};
    Customer(const string &email);
    void print();
    void save();
    bool validateEmail(const string &email);
    bool updateEmail(const string &new_email);
    void updatePhone(sqlite3 *db, long long new_phone);
    void updateAge(sqlite3 *db, int new_age);
    void updateName(sqlite3 *db, const string& new_name);
    bool updateContactInfo(long long new_phone, string &new_email);

    int getAge() const
    {
        return this->age;
    }
    string getName() const
    {
        return this->name;
    }
    int getId() const
    {
        return this->id;
    }
    int getPhone() const
    {
        return this->phone;
    }
    string getEmail() const
    {
        return this->email;
    }
};