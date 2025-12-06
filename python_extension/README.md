# Python C Extension Example

A comprehensive, well-documented example of creating Python classes in C using the Python/C API. This project serves as both a working implementation and an educational resource for understanding Python C extensions.

## 📚 Project Files

| File | Purpose |
|------|---------|
| `countermodule.c` | **Main C extension** - Heavily documented with 7 clear sections |
| `TUTORIAL.md` | **Deep dive tutorial** - Architecture, patterns, and advanced topics |
| `test_counter.py` | **Comprehensive tests** - Demonstrates all features with explanations |
| `setup.py` | Build configuration for setuptools |
| `Makefile` | Convenient build commands |

## 🎯 What This Demonstrates

The Counter class showcases essential Python C extension concepts:

### Core Concepts
- ✅ **Object Lifecycle**: `__new__`, `__init__`, and `__del__` implementation
- ✅ **Method Implementation**: Exposing C functions as Python methods
- ✅ **Attribute Access**: Direct access to C struct members from Python
- ✅ **Argument Parsing**: Converting Python arguments to C types
- ✅ **Reference Counting**: Proper memory management
- ✅ **Type Definition**: Creating Python types from C structs

### Python Equivalent

```python
class Counter:
    def __init__(self, initial_value=0, step=1):
        self.count = initial_value
        self.step = step
    
    def increment(self):
        self.count += self.step
    
    def decrement(self):
        self.count -= self.step
    
    def get_count(self):
        return self.count
    
    def reset(self):
        self.count = 0
    
    def set_step(self, new_step):
        self.step = new_step
```

## 🚀 Quick Start

### 1. Build the Extension

```bash
# Recommended: Build in-place for development
make build
# OR: python3 setup.py build_ext --inplace

# Alternative: Install system-wide
make install
# OR: python3 setup.py install --user
```

### 2. Run Tests

```bash
make test
# OR: python3 test_counter.py
```

### 3. Use in Python

After building, import and use the Counter class:

```python
import countermodule

# Create a counter starting at 10 with step 5
counter = countermodule.Counter(10, 5)

print(f"Initial count: {counter.get_count()}")  # 10

counter.increment()
print(f"After increment: {counter.get_count()}")  # 15

counter.decrement()
print(f"After decrement: {counter.get_count()}")  # 10

# Access attributes directly
print(f"Current count: {counter.count}")  # 10
print(f"Current step: {counter.step}")    # 5

counter.set_step(2)
counter.increment()
print(f"After changing step: {counter.get_count()}")  # 12

counter.reset()
print(f"After reset: {counter.get_count()}")  # 0
```

## 📖 Learning Path

### 1. Start Here: Read the Code
Open `countermodule.c` and read through the **7 sections**:
1. Type Definition (the struct)
2. Object Lifecycle (`__new__`, `__init__`, `__del__`)
3. Counter Methods (the actual functionality)
4. Method and Member Tables (exposing to Python)
5. Type Object Definition (the "class")
6. Module Definition (the import)
7. Module Initialization (tying it all together)

### 2. Deep Dive: Tutorial
Read `TUTORIAL.md` for:
- Architecture diagrams
- Reference counting explained
- Argument parsing formats
- Common patterns and pitfalls
- Advanced topics (properties, comparisons, etc.)

### 3. Practice: Modify and Extend
Try these exercises:
- Add a `multiply()` method
- Add input validation (reject negative steps)
- Implement `__repr__` and `__str__`
- Add comparison operators (`<`, `==`, etc.)
- Make it thread-safe with GIL management

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

## 🛠️ Build Commands

```bash
make build           # Build extension in-place
make test            # Run Python tests
make clean           # Remove build artifacts
make help            # Show all available commands
```

## 📚 Documentation Suite

This project includes comprehensive documentation for all skill levels:

| Document | Size | Best For | Contents |
|----------|------|----------|----------|
| **[README.md](README.md)** (this file) | 5KB | Getting started | Quick start, overview, basic usage |
| **[INDEX.md](INDEX.md)** | 8KB | Navigation | Complete documentation guide and finder |
| **[TUTORIAL.md](TUTORIAL.md)** | 14KB | Learning concepts | Architecture, patterns, reference counting, advanced topics |
| **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** | 11KB | Development | API cheat sheet, format codes, common patterns |
| **[ARCHITECTURE.md](ARCHITECTURE.md)** | 28KB | Visual learners | Diagrams, flows, memory layouts, complete architecture |
| **[REFACTORING_SUMMARY.md](REFACTORING_SUMMARY.md)** | - | Understanding changes | What was improved and why |

**👉 Start here**: [`INDEX.md`](INDEX.md) - Your guide to navigating all documentation

## 🌐 Official Python Documentation

- [Extending Python with C or C++](https://docs.python.org/3/extending/extending.html)
- [Defining Extension Types: Tutorial](https://docs.python.org/3/extending/newtypes_tutorial.html)
- [Defining Extension Types: Assorted Topics](https://docs.python.org/3/extending/newtypes.html)
- [Python/C API Reference Manual](https://docs.python.org/3/c-api/index.html)
