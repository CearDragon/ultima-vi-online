## Plan: Modernize Server Refactoring

Refactor the `src/server` files to adhere to modern C++ standards, improve readability, and replace unsafe raw memory management with well-encapsulated classes, collections, and smart pointers.

### Steps
1. Reorganize headers: Use `#pragma once` in `define_host.h` and convert legacy `#define` values to modern `constexpr` types and `enum class` definitions.
2. Encapsulate global state: Refactor the raw `extern` arrays in `data_host.h` into a localized `ServerState` class using modern `std::array` and `std::vector` structures with descriptive names.
3. Modernize memory management: Replace `malloc` and raw `free` pointer cleanups in `function_host.cpp` with RAII principles and `std::unique_ptr` architectures.
4. Refactor function signatures: Update legacy signatures in `function_host.h` to utilize meaningful camelCase naming conventions and pass standard library structures (e.g., `std::string_view`) rather than raw byte buffers.
5. Modularize game logic: Break down monolithic routines like `addu6monsterdropitems` in `function_host.cpp` into smaller focused domains such as `InventoryManager` and `EntityManager`.
6. .inc files containing implementation code should be entirely eliminated. Instead, they should be transitioned into:
   Proper .cpp Implementation Files: Abstract the logic into self-contained source files that have associated .h (or .hpp) headers with cleanly scoped functions.
   Object-Oriented Managers/Systems: Break massive chunks of loop and setup logic into specialized classes. .inc files containing implementation code should be entirely eliminated. Instead, they should be transitioned into:
   Proper .cpp Implementation Files: Abstract the logic into self-contained source files that have associated .h (or .hpp) headers with cleanly scoped functions.
   Object-Oriented Managers/Systems: Break massive chunks of loop and setup logic into specialized classes.
7. No magic numbers or strings.

