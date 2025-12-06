# Counter C Extension

A simple Python C extension that demonstrates basic counter functionality using the Python/C API.

## Overview

This module provides a Counter class implemented in pure C that can:
- Increment and decrement values
- Get the current counter value
- Reset the counter to zero

## Files

- `countermodule.c` - C implementation of the Counter class
- `setup.py` - Build configuration for the extension
- `test_counter.py` - Unit tests for the Counter class

## Building

Build and install the extension in development mode:

```bash
python setup.py develop
```

Or build in-place:

```bash
python setup.py build_ext --inplace
```

## Testing

Run the test suite:

```bash
python test_counter.py
```

## Usage

```python
from counter import Counter

# Create a counter
c = Counter()

# Increment the counter
c.increment()
c.increment()

# Get the current value
print(c.value())  # Output: 2

# Decrement
c.decrement()
print(c.value())  # Output: 1

# Reset
c.reset()
print(c.value())  # Output: 0
```

## Learning Points

This example demonstrates:
- Defining custom types in C extensions
- Implementing methods for Python objects in C
- Memory management with `tp_new` and `tp_dealloc`
- Proper reference counting
- Module initialization for Python 3
