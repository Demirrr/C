# Python Extensions Tutorial

A comprehensive guide to creating Python extensions in both C and C++.

## Overview

This repository contains examples and tutorials for building Python extensions using:
- **C Extensions** - Using the Python/C API
- **C++ Extensions** - Using C++ with the Python/C API

Python extensions allow you to:
- Write performance-critical code in C/C++
- Interface with existing C/C++ libraries
- Bypass the Global Interpreter Lock (GIL) for parallel processing
- Implement custom types and data structures efficiently

## Repository Structure

```
python_extension/
├── c_extension/           # C-based Python extensions
│   └── counter/          # Simple counter implementation in C
│       ├── countermodule.c
│       ├── setup.py
│       ├── test_counter.py
│       └── README.md
│
├── cpp_extensions/        # C++-based Python extensions
│   └── threadcounter/    # Thread-safe counter implementation in C++
│       ├── threadcounter.cpp
│       ├── setup.py
│       ├── test_threadcounter.py
│       └── README.md
│
├── ARCHITECTURE.md        # Deep dive into extension architecture
├── TUTORIAL.md           # Step-by-step tutorial for beginners
├── QUICK_REFERENCE.md    # Quick reference guide
└── INDEX.md              # Documentation index
```

## 📚 Documentation Files

| File | Purpose |
|------|---------|
| `ARCHITECTURE.md` | Deep dive into extension architecture and design patterns |
| `TUTORIAL.md` | Step-by-step tutorial for beginners |
| `QUICK_REFERENCE.md` | Quick reference for common tasks |
| `INDEX.md` | Complete documentation index |
| `Makefile` | Convenient build commands |

## Getting Started

### Prerequisites

- Python 3.x with development headers
- GCC or Clang compiler
- Make (optional, for Makefile-based builds)

### Quick Start

1. **C Extension (Counter)**
   ```bash
   cd c_extension/counter
   python setup.py develop
   python test_counter.py
   ```

2. **C++ Extension (Thread Counter)**
   ```bash
   cd cpp_extensions/threadcounter
   python setup.py develop
   python test_threadcounter.py
   ```

## Examples

### C Extension Example

The counter module demonstrates:
- Basic Python/C API usage
- Custom type definition
- Method implementation
- Memory management

```python
from counter import Counter

c = Counter()
c.increment()
print(c.value())  # Output: 1
```

### C++ Extension Example

The threadcounter module demonstrates:
- C++ integration with Python
- Thread-safe operations using mutexes
- Object-oriented design in extensions
- Advanced memory management

```python
from threadcounter import ThreadCounter

tc = ThreadCounter()
tc.increment()
print(tc.value())  # Output: 1
```

## 🎯 What These Extensions Demonstrate

### Core Concepts (Both C and C++)
- ✅ **Object Lifecycle**: `__new__`, `__init__`, and `__del__` implementation
- ✅ **Method Implementation**: Exposing C/C++ functions as Python methods
- ✅ **Attribute Access**: Direct access to C/C++ struct/class members from Python
- ✅ **Argument Parsing**: Converting Python arguments to C/C++ types
- ✅ **Reference Counting**: Proper memory management
- ✅ **Type Definition**: Creating Python types from C structs or C++ classes

### Advanced Concepts (C++)
- ✅ **Thread Safety**: Using mutexes for thread-safe operations
- ✅ **C++ Integration**: Wrapping C++ classes for Python
- ✅ **RAII Patterns**: Resource management using C++ idioms

## 📖 Learning Path

### 1. Start with C Extension
Begin with `c_extension/counter/` to understand:
- Basic Python/C API
- Type definitions and method tables
- Memory management fundamentals
- Module initialization

### 2. Progress to C++ Extension
Move to `cpp_extensions/threadcounter/` to learn:
- C++ class wrapping
- Thread safety considerations
- Advanced patterns and practices

### 3. Deep Dive: Read the Documentation
- **`TUTORIAL.md`** - Architecture, patterns, and advanced topics
- **`ARCHITECTURE.md`** - In-depth architecture and design patterns
- **`QUICK_REFERENCE.md`** - Quick reference for common tasks

### 4. Practice: Modify and Extend
Try these exercises:
- Add new methods to existing extensions
- Create your own extension from scratch
- Implement thread-safe operations
- Add input validation and error handling
- Implement special Python methods (`__repr__`, `__str__`, comparison operators)

## Key Concepts

### Python/C API
- Reference counting and memory management
- Type objects and method definitions
- Module initialization
- Error handling

### Building Extensions
- Using `setup.py` with `setuptools`
- Compiler flags and options
- Development mode installation
- Distribution and packaging

### Best Practices
- Proper reference counting (Py_INCREF/Py_DECREF)
- Error handling with PyErr_*
- Thread safety considerations
- Memory leak prevention

## Common Patterns

### 1. Creating a New Type
```c
static PyTypeObject MyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "mymodule.MyType",
    .tp_basicsize = sizeof(MyObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = MyType_new,
    .tp_dealloc = (destructor)MyType_dealloc,
    .tp_methods = MyType_methods,
};
```

### 2. Defining Methods
```c
static PyMethodDef MyType_methods[] = {
    {"method_name", (PyCFunction)method_func, METH_NOARGS, "docstring"},
    {NULL}
};
```

### 3. Module Initialization
```c
static struct PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    "mymodule",
    "module docstring",
    -1,
    NULL
};

PyMODINIT_FUNC PyInit_mymodule(void) {
    // Module initialization code
}
```

## 🔑 Key Concepts Reference

| Concept | What It Does |
|---------|--------------|
| `PyObject_HEAD` | Required header for all Python objects |
| `tp_new` | Memory allocation (`__new__`) |
| `tp_init` | Initialization with args (`__init__`) |
| `tp_dealloc` | Cleanup and free memory (`__del__`) |
| `PyMethodDef` | Maps Python method names to C functions |
| `PyMemberDef` | Exposes C struct members as Python attributes |
| `PyArg_ParseTuple` | Converts Python arguments to C types |
| `PyModule_Create` | Creates the module object |
| `Py_INCREF`/`Py_DECREF` | Reference counting for memory management |
| `Py_RETURN_NONE` | Returns None (with proper refcount) |

## Performance Tips

1. **Minimize Python/C boundary crossings** - Batch operations when possible
2. **Release the GIL** - Use `Py_BEGIN_ALLOW_THREADS` for CPU-bound work
3. **Use efficient data structures** - Leverage C/C++ data structures
4. **Profile your code** - Use cProfile and memory profilers
5. **Consider Cython** - For simpler cases, Cython may be easier

## Debugging

### Building with Debug Symbols
```bash
python setup.py build_ext --inplace --debug
```

### Using GDB
```bash
gdb python
(gdb) run test_counter.py
```

### Memory Leak Detection
```bash
valgrind --leak-check=full python test_counter.py
```

## 🛠️ Build Commands

```bash
make build           # Build extension in-place
make test            # Run Python tests
make clean           # Remove build artifacts
make help            # Show all available commands
```

## Resources

- [Python/C API Documentation](https://docs.python.org/3/c-api/)
- [Extending and Embedding Python](https://docs.python.org/3/extending/)
- [Python Extension Patterns](https://pythonextensionpatterns.readthedocs.io/)
- [Defining Extension Types: Tutorial](https://docs.python.org/3/extending/newtypes_tutorial.html)

## Contributing

Feel free to add more examples and improve documentation!

## License

This project is provided as educational material for learning Python extensions.
