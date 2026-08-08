#include <iostream>
#include "database.hpp"
#include "common.hpp"

using namespace std;

void afterLogin(sqlite3 *db);
void removeWS(string &text);

void runAdminCLI(sqlite3 *db)
{
    bool run = true;
    while (run)
    {
        string password;
        cout << "Enter admin password: ";
        getline(cin >> ws, password);
        if (password == "root")
        {
            cout << "Logged in as root" << endl;

            afterLogin(db);
        }
        else
        {
            cout << "Wrong password try again" << endl;
        }
    }
}

int itemsCallback(void *data, int argc, char **argv, char **azColName)
{
    auto *totalInventoryValue = static_cast<long long *>(data);

    if (argv[0] != nullptr)
    {
        *totalInventoryValue = stod(argv[0]);
    }
    else
    {
        *totalInventoryValue = 0;
    }

    return 0;
}

long long calculateTotalValue(sqlite3 *db)
{
    long long value = 0;
    const char *sql = R"(
        SELECT SUM(price * quantity) FROM items;
    )";
    char *errMsg = nullptr;

    int rc = sqlite3_exec(db, sql, itemsCallback, &value, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return value;
}

void afterLogin(sqlite3 *db)
{
    string option;
    bool run = true;
    while (run)
    {
        cout << "What do you want to do today :" << endl;
        cout << "1. Calculate Inventory worth" << endl;
        cout << "2. Create a user" << endl;
        cout << "3. Delete a user" << endl;
        cout << "4. Change user info/creds" << endl;
        cout << "Enter option :" << endl;
        getline(cin >> ws, option);
        removeWS(option);

        switch (stoi(option))
        {
        case 1:
        {
            long long value = 0;
            value = calculateTotalValue(db);
            cout << "Current inventory value: $" + to_string(value) << endl;
            break;
        }
        case 2:
        {
            string name, email, age, phone, password;
            Customer verify(email);
            cout << "Enter an account name :";
            getline(cin >> ws, name);

            cout << "Enter your email :";
            getline(cin >> ws, email);
            removeWS(email);
            if (!verify.validateEmail(email))
            {
                cout << "Enter a valid email" << endl;
                break;
            }
            else if (checkEmail(db, email))
            {
                cout << "Email already exists, only unique emails are allowed" << endl;
                break;
            }

            cout << "Enter your age :";
            getline(cin >> ws, age);
            removeWS(age);
            if (stoi(age) < 12 || stoi(age) > 150)
            {
                cout << "Enter a valid age" << endl;
                break;
            }

            cout << "Enter your phone number :";
            getline(cin >> ws, phone);
            removeWS(phone);
            if (stoll(phone) < 1000000000 && stoll(phone) > 9999999999)
            {
                cout << "Invalid Phone number, must be 10 digits only" << endl;
                break;
            }

            cout << "Enter your password :";
            getline(cin >> ws, password);

            insertCustomer(db, name, email, stoll(phone), stoi(age), password);
            cout << "Created User " + name + "!" << endl;
            break;
        }

        case 3:
        {
            bool run_temp = true;
            string email;
            while (run_temp)
            {
                cout << "Enter email of the user to delete: ";
                getline(cin >> ws, email);
                removeWS(email);
                if (checkEmail(db, email))
                {
                    auto name = getName(db, email);
                    if (name)
                    {
                        cout << "Do you want to delete: " + *name << endl;
                        string choice;
                        getline(cin >> ws, choice);
                        removeWS(choice);
                        if (choice == "yes" || choice == "y")
                        {
                            deleteUser(db, email);
                        }
                        else
                        {
                            run_temp = false;
                        }
                    }
                    else
                    {
                        cerr << "Error fetching name" << endl;
                        run_temp=false;
                    }
                }
                else
                {
                    cout << "Email doesn't exists" << endl;
                }
            }
            break;
        }

        case 4:
        {
            bool run1 = true;
            while (run1)
            {
                string email;
                cout << "Enter user's email: ";
                getline(cin >> ws, email);
                if (checkEmail(db, email))
                {
                    Customer user;
                    auto temp = fetchCustomerByEmail(db, email);
                    user = temp.value();

                    string new_phone, new_age, new_name, new_email, new_password;
                    bool input = true;
                    while (input)
                    {
                        cout << R"(Enter New Phone Number(enter "-" if not): )";
                        getline(cin >> ws, new_phone);
                        removeWS(new_phone);

                        if (new_phone != "-")
                        {
                            if (stoll(new_phone) < 1000000000 || stoll(new_phone) > 9999999999)
                            {
                                cout
                                    << "Invalid Phone Number" << endl;
                            }
                            else
                            {
                                bool isPhone = checkPhone(db, stoll(new_phone));
                                if (isPhone)
                                {
                                    cout << "Phone number already used pls use another one" << endl;
                                }
                                else
                                {
                                    user.updatePhone(db, stoll(new_phone));
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
                        getline(cin >> ws, new_age);
                        removeWS(new_age);

                        if (new_age != "-")
                        {
                            if (stoi(new_age) < 12 || stoi(new_age) > 150)
                            {
                                cout
                                    << "Please enter a valid age" << endl;
                            }
                            else
                            {
                                user.updateAge(db, stoi(new_age));
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
                        getline(cin >> ws, new_name);
                        removeWS(new_name);

                        if (new_name != "-")
                        {
                            user.updateName(db, new_name);
                            input = false;
                        }
                        else
                        {
                            input = false;
                        }
                    }

                    run = true;
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
                        else
                        {
                            run = false;
                        }
                    }
                    run1 = false;
                    run = true;
                }
                else
                {
                    cout << "Email doesn't exist" << endl;
                }
            }
            break;
        }
        case 5:
        {
            break;
        }
        default:
        {
            cout << "Enter a valid option" << endl;
            break;
        }
        }
    }
}
