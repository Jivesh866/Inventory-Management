cmake_minimum_required(VERSION 3.16)
project(InventoryApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
    GIT_TAG v5.0.0   
)
FetchContent_MakeAvailable(ftxui)

add_executable(InventoryApp
    src/main.cpp
    src/customer.cpp
    src/item.cpp
    src/database.cpp
    src/order.cpp
    src/clientCLI.cpp
    src/adminCLI.cpp
    src/common.cpp
)

target_include_directories(InventoryApp PUBLIC include)

find_package(SQLite3 REQUIRED)
target_include_directories(InventoryApp PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(InventoryApp PRIVATE ${SQLite3_LIBRARIES})

target_link_libraries(InventoryApp PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)