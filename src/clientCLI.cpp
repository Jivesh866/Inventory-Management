#include <iostream>
#include "database.hpp"
#include "customer.hpp"
#include "common.hpp"

using namespace std;

void afterLogin(sqlite3 *db, Customer user);
void removeWS(string &text);

void runClientCLI(sqlite3 *db)
{
    bool run = true;
    while (run)
    {
        cout << "What will you do today!" << endl;
        cout << "1. Create account" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Option: ";
        string choice, name, email, phone, age, password;
        Customer verify;
        getline(cin >> ws, choice);
        removeWS(choice);
        switch (stoi(choice))
        {
        case 1:
        {
            cout << "Enter an account name :";
            getline(cin >> ws, name);

            cout
                << "Enter your email :";
            getline(cin >> ws, email);
            removeWS(email);
            if (!verify.validateEmail(email))
            {
                cout
                    << "Enter a valid email" << endl;
                break;
            }
            else if (checkEmail(db, email))
            {
                cout
                    << "Email already exists, only unique emails are allowed" << endl;
                break;
            }

            cout
                << "Enter your age :";
            getline(cin >> ws, age);
            removeWS(age);
            if (stoi(age) < 12 || stoi(age) > 150)
            {
                cout
                    << "Enter a valid age" << endl;
                break;
            }

            cout
                << "Enter your phone number :";
            getline(cin >> ws, phone);
            removeWS(phone);
            if (stoll(phone) < 1000000000 && stoll(phone) > 9999999999)
            {
                cout
                    << "Invalid Phone number, must be 10 digits only" << endl;
                break;
            }

            cout
                << "Enter your password :";
            getline(cin >> ws, password);

            insertCustomer(db, name, email, stoll(phone), stoi(age), password);
            cout
                << "Created the user" << endl;
            auto id = getId(db, email);
            if (id)
            {
                Customer user(id.value(), name, email, stoll(phone), stoi(age));
                afterLogin(db, user);
            }
            else
            {
                cout
                    << "Unable to fetch id";
                run = false;
            }
            break;
        }

        case 2:
        {
            string email, password;
            cout << "Enter your email: ";
            getline(cin >> ws, email);
            removeWS(email);

            if (checkEmail(db, email))
            {
                cout
                    << "Enter your password: ";
                getline(cin >> ws, password);
                removeWS(password);

                if (checkPassword(db, email, password))
                {
                    auto name = getName(db, email);
                    if (name)
                    {
                        cout
                            << "Successfully logged in as " + *name << endl;

                        auto user = fetchCustomerByEmail(db, email);
                        if (user)
                        {
                            afterLogin(db, user.value());
                        }
                        else
                        {
                            cout << "Unable to fetch the user" << endl;
                        }
                    }
                    else
                    {
                        cout
                            << "Error getting the name" << endl;
                    }
                }
                else
                {
                    cout << "Incorrect Password" << endl;
                }
            }
            else
            {
                cout << "No user with this email exists" << endl;
            }

            break;
        }

        case 3:
        {
            run = false;
            break;
        }

        default:
            cout << "Please choose a valid option!" << endl;
            break;
        }
    }
}

void afterLogin(sqlite3 *db, Customer user)
{
    bool run = true;
    while (run)
    {
        cout << "What do you want to do today:" << endl;
        cout << "1. Create an order" << endl;
        cout << "2. Cancel an order" << endl;
        cout << "3. Update Info" << endl;
        cout << "4. Update Credentials" << endl;
        cout << "5. Close this account" << endl;
        cout << "6. Logout" << endl;
        cout << "Option : ";
        string choice;
        getline(cin >> ws, choice);
        removeWS(choice);

        switch (stoi(choice))
        {
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            string phone = "", age = "", name = "";
            bool input = true;
            while (input)
            {
                cout << R"(Enter New Phone Number(enter "-" if not): )";
                getline(cin >> ws, phone);
                removeWS(phone);

                if (phone != "-")
                {
                    if (stoll(phone) < 1000000000 || stoll(phone) > 9999999999)
                    {
                        cout
                            << "Invalid Phone Number" << endl;
                    }
                    else
                    {
                        bool isPhone = checkPhone(db, stoll(phone));
                        if (isPhone)
                        {
                            cout << "Phone number already used pls use another one" << endl;
                        }
                        else
                        {
                            user.updatePhone(db, stoll(phone));
                            input = false;
                        }
                    }
                }
                else
                {
                    input = false;
                }
            }
            input = true;
            while (input)
            {
                cout << R"(Enter new age(enter "-" if not) : )";
                getline(cin >> ws, age);
                removeWS(age);

                if (age != "-")
                {
                    if (stoi(age) < 12 || stoi(age) > 150)
                    {
                        cout
                            << "Please enter a valid age" << endl;
                    }
                    else
                    {
                        user.updateAge(db, stoi(age));
                        input = false;
                    }
                }
                else
                {
                    input = false;
                }
            }
            input = true;
            while (input)
            {
                cout << R"(Enter new name(enter "-" if not) : )";
                getline(cin >> ws, name);
                removeWS(name);

                if (name != "-")
                {
                    user.updateName(db, name);
                    input = false;
                }
                else
                {
                    input = false;
                }
            }
            break;
        }
        case 4:
        {
            string new_email, new_password;
            bool run = true;
            while (run)
            {
                cout << "Enter new email(enter - if not) :";
                getline(cin >> ws, new_email);
                removeWS(new_email);

                if (new_email != "-")
                {
                    if (checkEmail(db, new_email))
                    {
                        cout << "Enter a unique email" << endl;
                    }
                    else
                    {
                        if (user.updateEmail(db, new_email))
                        {
                            cout << "Email update Successfully" << endl;
                            run = false;
                        }
                        else
                        {
                            cout << "Failed to update email" << endl;
                        }
                    }
                }
                else
                {
                    run = false;
                }
            }

            run = true;
            while (run)
            {
                cout << "Enter new password(enter - if not) :";
                getline(cin >> ws, new_password);
                removeWS(new_password);
                if (new_password != "-")
                {
                    user.updatePassword(db, new_password);
                    run = false;
                }
                else{
                    run=false;
                }
            }

            break;
        }
        case 5:
        {
            user.deleteUser(db);
            run=false;
            break;
        }
        case 6:
        {
            run = false;
            break;
        }
        default:
        {
            cout << "Please enter a valid option";
            break;
        }
        }
    }
}
