# Python C Extension Tutorial

## Understanding the Architecture

This project demonstrates how to create Python classes in C. Here's the complete picture:

```
┌─────────────────────────────────────────────────────────────┐
│                        Python Layer                          │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  import countermodule                                │   │
│  │  counter = countermodule.Counter(10, 5)             │   │
│  │  counter.increment()  # Calls C code                │   │
│  └─────────────────────────────────────────────────────┘   │
└──────────────────────┬──────────────────────────────────────┘
                       │ Python/C API Bridge
┌──────────────────────▼──────────────────────────────────────┐
│                         C Layer                              │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  CounterObject struct                                 │  │
│  │  - PyObject_HEAD (ref count, type info)              │  │
│  │  - int count                                          │  │
│  │  - int step                                           │  │
│  └───────────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  Counter_increment(), Counter_decrement(), etc.      │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## The Seven Sections Explained

### Section 1: Type Definition (CounterObject)

This is your data structure - what gets stored in memory for each Counter instance.

```c
typedef struct {
    PyObject_HEAD     // MUST be first!
    int count;        // Your data
    int step;
} CounterObject;
```

**Key Point**: `PyObject_HEAD` contains Python's internal bookkeeping (reference count, type pointer). It MUST be the first member.

### Section 2: Object Lifecycle

Python creates objects in two stages:

1. **`__new__` (Counter_new)**: Allocates memory
   - Like `malloc()` but Python-aware
   - Sets safe default values
   - Returns the empty object

2. **`__init__` (Counter_init)**: Initializes with user values
   - Parses arguments from Python
   - Sets the actual starting values
   - Can fail (returns -1)

3. **`__del__` (Counter_dealloc)**: Cleanup
   - Frees resources
   - Decrements reference counts of owned objects
   - Called automatically by garbage collector

**Why two stages?**
- Separation of concerns: allocation vs initialization
- Allows unpickling, copying without calling `__init__`
- Matches Python's object model

### Section 3: Methods

Methods are C functions that follow a specific signature:

```c
static PyObject *
Counter_increment(CounterObject *self, PyObject *Py_UNUSED(ignored))
{
    self->count += self->step;
    Py_RETURN_NONE;
}
```

**Key Points**:
- Always return `PyObject*` (even for void methods - return None)
- First parameter is always `self`
- Use `Py_RETURN_NONE` for void methods
- For methods with arguments, use `PyArg_ParseTuple()`

### Section 4: Method and Member Tables

These are lookup tables that tell Python about your class:

**Method Table**: Maps Python method names to C functions
```c
static PyMethodDef Counter_methods[] = {
    {"increment", Counter_increment, METH_NOARGS, "docstring"},
    {NULL, NULL, 0, NULL}  // Sentinel
};
```

**Member Table**: Exposes C struct members directly
```c
static PyMemberDef Counter_members[] = {
    {"count", T_INT, offsetof(CounterObject, count), 0, "doc"},
    {NULL}  // Sentinel
};
```

This enables: `counter.count` instead of `counter.get_count()`

### Section 5: Type Object (CounterType)

This is the "class definition" - it tells Python everything about the Counter type:

```c
static PyTypeObject CounterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "countermodule.Counter",
    .tp_basicsize = sizeof(CounterObject),
    .tp_new = Counter_new,
    .tp_init = Counter_init,
    .tp_dealloc = Counter_dealloc,
    .tp_methods = Counter_methods,
    .tp_members = Counter_members,
    // ... many other slots available
};
```

**Available Slots** (we only use a few):
- `tp_repr`: Custom `__repr__` implementation
- `tp_str`: Custom `__str__` implementation
- `tp_richcompare`: Comparison operators (`<`, `==`, etc.)
- `tp_hash`: Make objects hashable
- `tp_iter`: Iterator protocol
- `tp_getattro`/`tp_setattro`: Custom attribute access

### Section 6: Module Definition

The module is the container - what you actually import:

```c
static PyModuleDef countermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "countermodule",
    .m_doc = "Module docstring",
    .m_size = -1,
};
```

A module can contain:
- Multiple classes (types)
- Module-level functions
- Constants
- Submodules

### Section 7: Module Initialization

This is where it all comes together:

```c
PyMODINIT_FUNC
PyInit_countermodule(void)
{
    // 1. Prepare the type
    PyType_Ready(&CounterType);
    
    // 2. Create the module
    m = PyModule_Create(&countermodule);
    
    // 3. Add type to module
    Py_INCREF(&CounterType);
    PyModule_AddObject(m, "Counter", (PyObject *) &CounterType);
    
    // 4. Return the module
    return m;
}
```

**CRITICAL**: Function name MUST be `PyInit_<modulename>`

## Reference Counting Deep Dive

Python uses reference counting for memory management. Every `PyObject*` has a count:

```c
// Creating a new Python integer
PyObject *num = PyLong_FromLong(42);  // refcount = 1 (NEW reference)

// Giving it to someone else
Py_INCREF(num);  // refcount = 2
some_function(num);  // They own a reference now

// Done with it
Py_DECREF(num);  // refcount = 1
// Someone else still has a reference

// When refcount reaches 0, Python calls tp_dealloc
```

**Rules**:
- Functions that return NEW references: `PyLong_FromLong()`, `PyUnicode_FromString()`, etc.
- Functions that return BORROWED references: `PyTuple_GetItem()`, `PyList_GetItem()`
- Functions that STEAL references: `PyModule_AddObject()` (on success)
- Always `Py_INCREF` before giving away a reference
- Always `Py_DECREF` when done with a reference

## Argument Parsing Format Strings

When parsing arguments from Python, format strings specify types:

```c
// Format string syntax
"|ii"   // All optional, two ints
"i"     // One required int
"ii"    // Two required ints
"i|i"   // First required, second optional
"s"     // String (char*)
"d"     // Double
"O"     // Any Python object (PyObject*)
"O!"    // Object of specific type
"(ii)"  // Tuple of two ints
"[ii]"  // List of two ints (rarely used)
```

Examples:
```c
int a, b;
PyArg_ParseTuple(args, "ii", &a, &b);  // Two ints required

char *name;
int age;
PyArg_ParseTuple(args, "si", &name, &age);  // String and int

PyObject *obj;
PyArg_ParseTuple(args, "O", &obj);  // Any Python object
```

## Common Patterns

### Pattern 1: Returning Python Objects from C

```c
// Return an integer
return PyLong_FromLong(42);

// Return a float
return PyFloat_FromDouble(3.14);

// Return a string
return PyUnicode_FromString("Hello");

// Return a list
PyObject *list = PyList_New(2);
PyList_SetItem(list, 0, PyLong_FromLong(1));
PyList_SetItem(list, 1, PyLong_FromLong(2));
return list;

// Return None
Py_RETURN_NONE;
```

### Pattern 2: Extracting C Values from Python Objects

```c
// Get integer from PyObject
long value = PyLong_AsLong(py_obj);
if (value == -1 && PyErr_Occurred()) {
    // Handle error
}

// Get string from PyObject
const char *str = PyUnicode_AsUTF8(py_obj);
if (str == NULL) {
    // Handle error
}
```

### Pattern 3: Raising Python Exceptions from C

```c
// Raise TypeError
PyErr_SetString(PyExc_TypeError, "Expected an integer");
return NULL;

// Raise ValueError
PyErr_SetString(PyExc_ValueError, "Value must be positive");
return NULL;

// Format error message
PyErr_Format(PyExc_ValueError, "Expected value > 0, got %d", value);
return NULL;
```

### Pattern 4: Checking for Errors

Many Python C API functions return NULL or -1 on error:

```c
PyObject *result = some_function();
if (result == NULL) {
    // Error occurred, exception is already set
    return NULL;  // Propagate the error
}

int status = some_other_function();
if (status == -1) {
    // Error occurred
    return NULL;
}
```

## Building and Testing

### Build Commands

```bash
# Build in development mode (creates .so in current directory)
python3 setup.py build_ext --inplace

# Install system-wide
python3 setup.py install --user

# Clean build artifacts
make clean

# Run tests
make test
```

### Debugging Tips

1. **Use print statements**: `printf()` works in C extensions
2. **Check return values**: Always check if functions return NULL
3. **Use gdb**: 
   ```bash
   gdb python3
   (gdb) run test_counter.py
   ```
4. **Enable core dumps**: `ulimit -c unlimited`
5. **Valgrind for memory leaks**:
   ```bash
   valgrind --leak-check=full python3 test_counter.py
   ```

## Advanced Topics

### Adding Properties (getters/setters)

Instead of direct member access, you can use properties:

```c
static PyObject *
Counter_get_count(CounterObject *self, void *closure)
{
    return PyLong_FromLong(self->count);
}

static int
Counter_set_count(CounterObject *self, PyObject *value, void *closure)
{
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "count must be an integer");
        return -1;
    }
    self->count = PyLong_AsLong(value);
    return 0;
}

static PyGetSetDef Counter_getsetters[] = {
    {"count", (getter)Counter_get_count, (setter)Counter_set_count,
     "count property", NULL},
    {NULL}
};

// In CounterType:
.tp_getset = Counter_getsetters,
```

### Adding Rich Comparison

```c
static PyObject *
Counter_richcompare(PyObject *self, PyObject *other, int op)
{
    if (!PyObject_TypeCheck(other, &CounterType)) {
        Py_RETURN_NOTIMPLEMENTED;
    }
    
    int count1 = ((CounterObject *)self)->count;
    int count2 = ((CounterObject *)other)->count;
    
    switch (op) {
        case Py_LT: return PyBool_FromLong(count1 < count2);
        case Py_LE: return PyBool_FromLong(count1 <= count2);
        case Py_EQ: return PyBool_FromLong(count1 == count2);
        case Py_NE: return PyBool_FromLong(count1 != count2);
        case Py_GT: return PyBool_FromLong(count1 > count2);
        case Py_GE: return PyBool_FromLong(count1 >= count2);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

// In CounterType:
.tp_richcompare = Counter_richcompare,
```

### Adding __repr__ and __str__

```c
static PyObject *
Counter_repr(CounterObject *self)
{
    return PyUnicode_FromFormat("Counter(count=%d, step=%d)",
                                self->count, self->step);
}

// In CounterType:
.tp_repr = (reprfunc)Counter_repr,
```

## Next Steps

1. **Add input validation**: Check for negative steps, overflow, etc.
2. **Add more methods**: `multiply()`, `divide()`, `set_count()`
3. **Implement comparison operators**: Make counters comparable
4. **Add properties**: Use getters/setters for validation
5. **Add module-level functions**: Utilities that don't need an instance
6. **Handle threading**: Add GIL management for threaded code
7. **Support pickling**: Implement `__reduce__` for serialization

## Common Errors and Solutions

### ImportError: dynamic module does not define module export function

**Problem**: Function name doesn't match module name  
**Solution**: Ensure `PyInit_<modulename>` matches your module name exactly

### SystemError: initialization of countermodule failed without raising an exception

**Problem**: Returning NULL without setting an exception  
**Solution**: Always call `PyErr_SetString()` before returning NULL

### Segmentation fault

**Problem**: Reference counting error or NULL pointer dereference  
**Solution**: Check all pointers, verify Py_INCREF/Py_DECREF calls

### Memory leaks

**Problem**: Created objects without decrementing their refcount  
**Solution**: Every Py_INCREF needs a matching Py_DECREF, use valgrind to detect

## Resources

- [Official Python C API Documentation](https://docs.python.org/3/c-api/index.html)
- [Extending and Embedding Tutorial](https://docs.python.org/3/extending/extending.html)
- [Defining Extension Types](https://docs.python.org/3/extending/newtypes.html)
- [Python/C API Reference Manual](https://docs.python.org/3/c-api/index.html)
