#include <iostream>
#include "database.hpp"

using namespace std;
void runAdminCLI(sqlite3* db){

}

int itemsCallback(void* data, int argc, char** argv, char** azColName){
auto* totalInventoryValue = static_cast<double*>(data);

    if (argv[0] != nullptr) {
        *totalInventoryValue = stod(argv[0]);
    } else {
        *totalInventoryValue = 0.0; 
    }

    return 0;
}

double calculateTotalValue(sqlite3* db){
    double value=0;
    const char* sql=R"(
        SELECT SUM(price * quantity) FROM items;
    )";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, sql, itemsCallback, &value, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return value;
}