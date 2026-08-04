#include <iostream>
#include "database.hpp"
#include "adminCLI.hpp"
#include "clientCLI.hpp"

using namespace std;

int main()
{
    sqlite3 *db = nullptr;
    if (sqlite3_open("inventory.db", &db) != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    initialize(db);

    bool run = true;
    while (run)
    {
        cout << "Choose a mode" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Client" << endl;
        cout << "3. Exit" << endl;
        string choice;
        getline(cin >> ws, choice);

        switch (stoi(choice))
        {
        case 1:
            runAdminCLI(db);
            break;

        case 2:
            runClientCLI(db);
            break;

        case 3:
            run = false;
            break;
        default:
            cout << "Choose a valid operation";
            break;
        }
    }
    
    sqlite3_close(db);
}