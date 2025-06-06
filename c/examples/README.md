# C GoFile Wrapper Examples

This directory contains example code demonstrating how to use the GoFile wrapper in C.

## Prerequisites

- C compiler (gcc or MSVC)
- libcurl development libraries
- Make (optional)

## Building the Example

### Using Make

```bash
make
```

### Manual Compilation

```bash
gcc -o example main.c ../gofile_wrapper.c -lcurl
```

## Running the Example

```bash
./example
```

Make sure to replace `path/to/your/file.txt` in the example code with the actual path to a file you want to upload.

## Memory Management

The example demonstrates proper memory management with the C wrapper:
- All responses from the wrapper functions must be freed using `gofile_free_response()`
- The client must be cleaned up using `gofile_cleanup()` 