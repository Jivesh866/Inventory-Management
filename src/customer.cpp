#include <iostream>
#include "customer.hpp"
#include <format>
#include <regex>
#include <stdexcept>

using namespace std;

Customer::Customer(int id, string& name, string& email, int phone, int age)
{

    if (!Customer::validateEmail(email))
    {
        throw invalid_argument("Invalid email: " + email);
    }
    else if (phone < 100000000)
    {
        throw invalid_argument("Invalid phone " + to_string(phone));
    }
    else
    {
        this->id = id;
        this->name = name;
        this->age = age;
        this->phone = phone;
        this->email = email;
    }
}
Customer::Customer(const string& email){
    this->email=email;
}


void Customer::print()
{
    cout << format("Name: {}, Age: {}, ID: {}", name, age, id);
}

void Customer::save()
{
}

bool Customer::validateEmail(const string& email)
{
    const regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailPattern);
}

void Customer::updatePhone(int new_phone)
{
    if (phone < 1000000000)
    {
        throw invalid_argument("Invalid phone: " + to_string(phone));
    }
    else
    {
        this->phone = new_phone;
    }
}

bool Customer::updateEmail(string& new_email)
{
    if (Customer::validateEmail(new_email))
    {
        this->email = new_email;
        return true;
    }
    else
    {
        return false;
    }
}

bool Customer::updateContactInfo(int new_phone, string& new_email)
{
    if (!Customer::validateEmail(email))
    {
        return false;
    }
    else if (phone < 100000000)
    {
        return false;
    }
    else
    {
        email=new_email;
        phone=new_phone;
        return true;
    }
}