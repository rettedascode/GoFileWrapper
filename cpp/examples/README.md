# C++ GoFile Wrapper Examples

This directory contains example code demonstrating how to use the GoFile wrapper in C++.

## Prerequisites

- C++ compiler (g++ or MSVC)
- libcurl development libraries
- CMake (recommended)

## Building the Example

### Using CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
make
```

### Manual Compilation

```bash
g++ -o example main.cpp ../GoFileWrapper.cpp -lcurl
```

## Running the Example

```bash
./example
```

Make sure to replace `path/to/your/file.txt` in the example code with the actual path to a file you want to upload. 