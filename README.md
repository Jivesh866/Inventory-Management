# Inventory Management

A command-line inventory management system written in **C++**, with separate **admin** and **client** roles. Work in progress.

## Features
- Admin interface: create and delete user accounts *(in progress)*
- Client interface: create and delete accounts and orders *(in progress)*
- Modular layout with headers in `include/` and implementations in `src/`
- CMake build system

## Project structure
```
Inventory-Management/
├── include/         # Header files
├── src/             # Source files
├── CMakeLists.txt   # Build configuration
├── TODO.txt         # Planned work
└── LICENSE          # Apache-2.0
```

## Getting started

### Prerequisites
- C++ compiler with C++17 support
- CMake 3.10+

### Build and run
```bash
git clone https://github.com/Jivesh866/Inventory-Management.git
cd Inventory-Management
mkdir build && cd build
cmake ..
make
./InventoryApp     
```

## Roadmap
- [x] Project skeleton and CMake setup
- [ ] Admin CLI: create/delete accounts
- [ ] Client CLI: create/delete accounts
- [ ] Client CLI: create/delete orders
- [ ] Persistent storage (file or database)
- [ ] Stock tracking and low-stock alerts
- [ ] Input validation and error handling

## License
Licensed under the [Apache License 2.0](LICENSE).

## Author
**Jivesh Bisht**: [GitHub](https://github.com/Jivesh866) · [LinkedIn](https://www.linkedin.com/in/jivesh-bisht-177414423/)
