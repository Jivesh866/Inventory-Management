#include <iostream>
#include "customer.hpp"
#include <format>
#include <regex>
#include <stdexcept>

using namespace std;

Customer::Customer(int id, const string &name, const string &email, long long phone, int age)
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
Customer::Customer(const string &email)
{
    this->email = email;
}

void Customer::print()
{
    cout << format("Name: {}, Age: {}, ID: {}", name, age, id);
}

void Customer::save()
{
}

bool Customer::validateEmail(const string &email)
{
    const regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, emailPattern);
}

void Customer::updatePhone(sqlite3 *db, long long new_phone)
{
    if (phone < 1000000000 || phone > 9999999999)
    {
        throw invalid_argument("Invalid phone: " + to_string(phone));
    }
    else
    {
        const char *sql = "UPDATE customers SET phone=? WHERE email=?";
        sqlite3_stmt *stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_bind_int64(stmt, 1, new_phone);
        sqlite3_bind_text(stmt, 2, this->email.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            this->phone = new_phone;
        }
        else
        {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
}

void Customer::updateAge(sqlite3 *db, int new_age)
{
    if (new_age < 12 || new_age > 150)
    {
        throw invalid_argument("Invalid Age: " + to_string(age));
    }
    else
    {
        const char *sql = "UPDATE customers SET age=? WHERE email=?";
        sqlite3_stmt *stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_bind_int(stmt, 1, new_age);
        sqlite3_bind_text(stmt, 2, this->email.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            this->age = new_age;
        }
        else
        {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
}

void Customer::updateName(sqlite3 *db, const string &new_name)
{
    const char *sql = "UPDATE customers SET name=? WHERE email=?";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_bind_text(stmt, 1, new_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, this->email.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        this->name = new_name;
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

bool Customer::updateEmail(const string &new_email)
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

bool Customer::updateContactInfo(long long new_phone, string &new_email)
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
        email = new_email;
        phone = new_phone;
        return true;
    }
}