#pragma once

#include <iostream>
using namespace std;

class Customer
{
private:
    int id;
    string name;
    string email;
    int phone;
    int age;

public:

    Customer(int id, string& name,string& email,int phone, int age);
    Customer(){};
    Customer(const string& email);
    void print();
    void save();
    bool validateEmail(const string& email);
    bool updateEmail(string& new_email);
    void updatePhone(int new_phone);
    bool updateContactInfo(int new_phone,string& new_email);
    
    
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