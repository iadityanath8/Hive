# Hive Library

Hive is a custom C++ standard library alternative designed to provide a more Python-like experience while maintaining high performance and flexibility. It aims to replace parts of the existing C++ standard library with a fresh, modern, and lightweight approach.

## Features

- **String Class**: A custom string implementation that provides efficient memory management and a user-friendly API.
- **List Class**: A dynamic array-like container with efficient resizing and element access.
- **Generic Print Function**: A Python-like `print` function that supports automatic spacing between arguments.
- **Format Function**: A lightweight, flexible alternative to `printf` that works with `const char*`.

## Goals

- **Minimal Dependency**: Hive does not rely on the C++ standard library (`std`) but utilizes the C standard library (`libc`).
- **Performance-Oriented**: Optimized memory management for speed and efficiency.
- **Python-Like Simplicity**: Inspired by Python’s ease of use while retaining the power of C++.
- **Custom Standard Library**: Aiming to replace key C++ standard library components with a custom alternative.

## Installation

To use Hive in your project, simply include the `hive.hpp` header and compile your code with a C++ compiler that supports C++20.

```sh
 g++ --std=c++20 -fno-exceptions -o main main.cc
```

## Usage

### String
```cpp
String s = "Hello, Hive!";
s += " Welcome.";
print(s);
```

### List
```cpp
List<int> numbers = {1, 2, 3, 4};
numbers.push(5);
print(numbers);
```

### Generic Print
```cpp
print("Hive is awesome!", 42, 3.14);
```

### Format
```cpp
String formatted = format("The answer is {}", 42);
print(formatted);
```

## Future Work
- Implementing additional data structures (e.g., `Map`, `Set`).
- Expanding the `format` function for more advanced formatting.
- Creating an optimized memory allocator.

## Contribution
Contributions are welcome! Feel free to submit issues, feature requests, or pull requests to improve Hive.

## License
Hive is an open-source project. The license will be determined in the future.

---
This project is a work in progress, and feedback is highly appreciated!

