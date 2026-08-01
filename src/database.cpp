#include <iostream>
#include <string>
#include "database.hpp"
#include <sqlite3.h>
#include <optional>

void initialize(sqlite3 *db)
{
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS customers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            phone INTEGER UNIQUE NOT NULL,
            age INTEGER NOT NULL
        );
    )";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table 1 created successfully.\n";
    }

    sql = R"(
    CREATE TABLE IF NOT EXISTS items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        sku TEXT UNIQUE NOT NULL,
        name TEXT NOT NULL,
        price INTEGER NOT NULL,
        quantity INTEGER NOT NULL DEFAULT 0,
        reorder_threshold INTEGER DEFAULT 5
    );
    
    
)";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table 2 created successfully.\n";
    }

    sql = R"(
    CREATE TABLE IF NOT EXISTS orders (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER NOT NULL,
    total_amount REAL NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (customer_id) REFERENCES customers(id)
);
    
    
    
)";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table 3 created successfully.\n";
    }

    sql = R"(
    CREATE TABLE IF NOT EXISTS order_items (
    order_id INTEGER NOT NULL,
    item_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    unit_price REAL NOT NULL,
    FOREIGN KEY (order_id) REFERENCES orders(id),
    FOREIGN KEY (item_id) REFERENCES items(id)
);
    
    
)";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table 4 created successfully.\n";
    }

    sql = R"(
    CREATE TABLE IF NOT EXISTS creds (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    email TEXT NOT NULL,
    password TEXT NOT NULL,
    FOREIGN KEY (email) REFERENCES customers(email)
);
    
    
)";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Table 5 created successfully.\n";
    }
}

void insertCustomer(sqlite3 *db, const std::string &name, const std::string &email, double phone, int age, const std::string &password)
{
    const char *sql = "INSERT INTO customers (name, email, phone, age) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt = nullptr;

    // 1. Prepare statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // 2. Bind parameters (1-indexed)
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, phone);
    sqlite3_bind_int(stmt, 4, age);

    // 3. Step (execute query)
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        std::cout << "Inserted customer successfully.\n";
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sql = "INSERT INTO creds (email, password) VALUES (?, ?);";
    stmt = nullptr;

    // 1. Prepare statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // 2. Bind parameters (1-indexed)
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    // 3. Step (execute query)
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        std::cout << "Inserted customer's password successfully.\n";
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    // 4. Clean up statement handle
    sqlite3_finalize(stmt);
}

void fetchCustomers(sqlite3 *db)
{
    const char *sql = "SELECT id, name, email, phone, age FROM customers;";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        // Step through result rows
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *name = sqlite3_column_text(stmt, 1);
            const unsigned char *email = sqlite3_column_text(stmt, 2);
            const int phone = sqlite3_column_int(stmt, 3);
            const int age = sqlite3_column_int(stmt, 4);

            std::cout << "ID: " << id
                      << " | Name: " << name
                      << " | Email: " << email
                      << " | Phone: " << phone
                      << " | Age: " << age << '\n';
        }
    }

    sqlite3_finalize(stmt);
}

// int main() {
//     sqlite3* db = nullptr;

//     // Open connection (creates file if it doesn't exist)
//     if (sqlite3_open("inventory.db", &db) != SQLITE_OK) {
//         std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
//         return 1;
//     }

//     initialize(db);
//     insertCustomer(db, "Alice Smith", "alice@example.com");
//     fetchCustomers(db);

//     // Close database connection
//     sqlite3_close(db);
//     return 0;
// }

int getItemPriceById(int item_id)
{
    int price = 0;

    sqlite3 *db = nullptr;
    if (sqlite3_open("inventory.db", &db) != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char *sql = R"(
        SELECT price FROM items WHERE id=?
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_bind_int(stmt, 1, item_id);

    char *errMsg = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        return sqlite3_column_int(stmt, 0);
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return price;
}

int getItemPriceBySku(std::string &sku)
{
    int price = 0;

    sqlite3 *db = nullptr;
    if (sqlite3_open("inventory.db", &db) != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char *sql = R"(
        SELECT price FROM items WHERE sku=?
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_bind_text(stmt, 1, sku.c_str(), -1, SQLITE_TRANSIENT);

    char *errMsg = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        return sqlite3_column_int(stmt, 0);
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return price;
}

int getItemQuantity(int item_id)
{
    int price = 0;
    sqlite3 *db = nullptr;
    if (sqlite3_open("inventory.db", &db) != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char *sql = R"(
        SELECT quantity FROM items WHERE id=?
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insert: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_bind_int(stmt, 1, item_id);

    char *errMsg = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        return sqlite3_column_int(stmt, 0);
    }
    else
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return price;
}

bool checkEmail(sqlite3 *db, const std::string &email)
{
    const char *sql = "SELECT EXISTS(SELECT 1 FROM customers WHERE LOWER(email) = LOWER(?1));";

    sqlite3_stmt *stmt = nullptr;
    bool exists = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = (sqlite3_column_int(stmt, 0) == 1);
    }
    else
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    return exists;
}

bool checkPassword(sqlite3 *db, const std::string &email, const std::string &password)
{
    const char *sql = "SELECT password FROM creds WHERE email=?;";
    std::string correct_password;
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text != nullptr)
        {
            correct_password = reinterpret_cast<const char *>(text);
        }
        else
        {
            correct_password = ""; // Or handle NULL value appropriately
        }
    }
    else
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_finalize(stmt);
    if (correct_password == password)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::optional<std::string> getName(sqlite3 *db, const std::string &email)
{
    const char *sql = "SELECT name FROM customers WHERE email = ?;";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return std::nullopt;
    }

    if (sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::optional<std::string> result = std::nullopt;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text != nullptr)
        {
            result = reinterpret_cast<const char *>(text);
        }
    }

    sqlite3_finalize(stmt);

    return result;
}