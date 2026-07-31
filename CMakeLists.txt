cmake_minimum_required(VERSION 3.16)
project(InventoryApp VERSION 1.0 LANGUAGES CXX)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable target and list source files
add_executable(InventoryApp
    src/main.cpp
    src/customer.cpp
    src/item.cpp
    src/database.cpp
    src/order.cpp
    src/clientCLI.cpp
    src/adminCLI.cpp
)

# Specify header search paths for your project
target_include_directories(InventoryApp PUBLIC include)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Include SQLite3 headers and link the library
target_include_directories(InventoryApp PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(InventoryApp PRIVATE ${SQLite3_LIBRARIES})