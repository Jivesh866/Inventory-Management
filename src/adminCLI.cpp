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
            cout << endl
                 << "Logged in as root" << endl;

            afterLogin(db);
        }
        else
        {
            cout << endl
                 << "Wrong password try again" << endl;
        }
    }
}

int itemsCallback(void *data, int argc, char **argv, char **azColName)
{
    auto *totalInventoryValue = static_cast<double *>(data);

    if (argv[0] != nullptr)
    {
        *totalInventoryValue = stod(argv[0]);
    }
    else
    {
        *totalInventoryValue = 0.0;
    }

    return 0;
}

double calculateTotalValue(sqlite3 *db)
{
    double value = 0;
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
        cout << "Enter option :" << endl;
        getline(cin >> ws, option);
        removeWS(option);
    }
}
