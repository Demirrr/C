# Python C Extension Quick Reference

A condensed cheat sheet for Python/C API development.

## 📋 Essential Headers

```c
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"  // For PyMemberDef
```

## 🏗️ Object Structure Template

```c
typedef struct {
    PyObject_HEAD          // MUST be first!
    /* Your data members */
    int my_int;
    char *my_string;
    PyObject *my_pyobj;    // For Python objects
} MyObject;
```

## 🔄 Lifecycle Functions

### Allocator (__new__)
```c
static PyObject *
My_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    MyObject *self = (MyObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        // Initialize with safe defaults
        self->my_int = 0;
        self->my_string = NULL;
        self->my_pyobj = NULL;
    }
    return (PyObject *)self;
}
```

### Initializer (__init__)
```c
static int
My_init(MyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"arg1", "arg2", NULL};
    int arg1 = 0;  // default
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &arg1))
        return -1;
    
    self->my_int = arg1;
    return 0;
}
```

### Destructor (__del__)
```c
static void
My_dealloc(MyObject *self)
{
    // Release any owned Python objects
    Py_XDECREF(self->my_pyobj);
    
    // Free any allocated memory
    if (self->my_string != NULL) {
        free(self->my_string);
    }
    
    // Free the object itself
    Py_TYPE(self)->tp_free((PyObject *)self);
}
```

## 📝 Method Signatures

### No Arguments (METH_NOARGS)
```c
static PyObject *
My_method(MyObject *self, PyObject *Py_UNUSED(ignored))
{
    // Do something
    Py_RETURN_NONE;
}
```

### With Arguments (METH_VARARGS)
```c
static PyObject *
My_method_args(MyObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    
    // Do something with value
    Py_RETURN_NONE;
}
```

### With Keywords (METH_VARARGS | METH_KEYWORDS)
```c
static PyObject *
My_method_kwargs(MyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"param1", "param2", NULL};
    int p1, p2 = 0;  // p2 optional
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|i", kwlist, &p1, &p2))
        return NULL;
    
    Py_RETURN_NONE;
}
```

## 🔤 Argument Parsing Format Codes

```c
// Integer types
"b"   // unsigned char
"h"   // short
"i"   // int
"l"   // long
"k"   // unsigned long
"n"   // Py_ssize_t
"L"   // long long

// Floating point
"f"   // float
"d"   // double

// String types
"s"   // char* (UTF-8)
"z"   // char* or NULL
"y"   // bytes (char*)
"u"   // Unicode (Py_UNICODE*)

// Objects
"O"   // PyObject*
"O!"  // PyObject* of specific type
"O&"  // PyObject* via converter function

// Modifiers
"|"   // All following are optional
"$"   // Keyword-only arguments follow
```

### Examples
```c
"i"      // One required int
"|i"     // One optional int
"ii"     // Two required ints
"i|i"    // One required, one optional int
"s"      // Required string
"si"     // Required string and int
"O"      // Any Python object
"O!"     // Object of specific type: PyArg_ParseTuple(args, "O!", &PyList_Type, &obj)
```

## 🔄 Type Conversions

### C to Python
```c
// Integers
PyLong_FromLong(42)
PyLong_FromLongLong(42LL)
PyLong_FromUnsignedLong(42U)

// Floats
PyFloat_FromDouble(3.14)

// Strings
PyUnicode_FromString("hello")
PyUnicode_FromFormat("Value: %d", 42)

// Bytes
PyBytes_FromString("data")
PyBytes_FromStringAndSize(data, len)

// Booleans
PyBool_FromLong(1)  // True
PyBool_FromLong(0)  // False

// None
Py_RETURN_NONE;  // Macro handles refcount

// Lists
PyObject *list = PyList_New(size);
PyList_SetItem(list, index, item);  // Steals reference to item

// Tuples
PyObject *tuple = PyTuple_New(size);
PyTuple_SetItem(tuple, index, item);  // Steals reference

// Dicts
PyObject *dict = PyDict_New();
PyDict_SetItemString(dict, "key", value);
```

### Python to C
```c
// Integers
long val = PyLong_AsLong(obj);
if (val == -1 && PyErr_Occurred()) { /* handle error */ }

// Floats
double val = PyFloat_AsDouble(obj);

// Strings
const char *str = PyUnicode_AsUTF8(obj);
if (str == NULL) { /* handle error */ }

// Check types
if (PyLong_Check(obj)) { /* is int */ }
if (PyFloat_Check(obj)) { /* is float */ }
if (PyUnicode_Check(obj)) { /* is str */ }
if (PyList_Check(obj)) { /* is list */ }
```

## 📊 Method and Member Tables

### Method Table
```c
static PyMethodDef My_methods[] = {
    {"method_name", (PyCFunction)My_method, METH_NOARGS,
     "Method docstring"},
    {"method_with_args", (PyCFunction)My_method_args, METH_VARARGS,
     "Another method"},
    {NULL, NULL, 0, NULL}  // Sentinel
};
```

### Member Table (Direct Access)
```c
static PyMemberDef My_members[] = {
    {"attribute", T_INT, offsetof(MyObject, my_int), 0,
     "Attribute docstring"},
    {"readonly_attr", T_INT, offsetof(MyObject, my_int), READONLY,
     "Read-only attribute"},
    {NULL, 0, 0, 0, NULL}  // Sentinel
};

// Member types:
T_SHORT, T_INT, T_LONG, T_FLOAT, T_DOUBLE
T_STRING, T_OBJECT, T_CHAR, T_BYTE, T_UBYTE
T_USHORT, T_UINT, T_ULONG, T_BOOL, T_LONGLONG
```

### Property Table (Getters/Setters)
```c
static PyGetSetDef My_getsetters[] = {
    {"property", (getter)My_get_prop, (setter)My_set_prop,
     "Property docstring", NULL},
    {NULL, NULL, NULL, NULL, NULL}  // Sentinel
};

static PyObject *
My_get_prop(MyObject *self, void *closure)
{
    return PyLong_FromLong(self->my_int);
}

static int
My_set_prop(MyObject *self, PyObject *value, void *closure)
{
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Expected int");
        return -1;
    }
    self->my_int = PyLong_AsLong(value);
    return 0;
}
```

## 🎯 Type Object Template

```c
static PyTypeObject MyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "mymodule.MyClass",
    .tp_doc = PyDoc_STR("Class docstring"),
    .tp_basicsize = sizeof(MyObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = My_new,
    .tp_init = (initproc)My_init,
    .tp_dealloc = (destructor)My_dealloc,
    .tp_methods = My_methods,
    .tp_members = My_members,
    .tp_getset = My_getsetters,
};
```

### Useful Type Slots
```c
.tp_repr = (reprfunc)My_repr,           // __repr__
.tp_str = (reprfunc)My_str,             // __str__
.tp_hash = (hashfunc)My_hash,           // __hash__
.tp_richcompare = My_richcompare,       // <, <=, ==, !=, >, >=
.tp_iter = (getiterfunc)My_iter,        // __iter__
.tp_iternext = (iternextfunc)My_next,   // __next__
```

## 📦 Module Definition

```c
static PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mymodule",
    .m_doc = "Module docstring",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_mymodule(void)  // MUST match module name!
{
    PyObject *m;
    
    // Ready the type
    if (PyType_Ready(&MyType) < 0)
        return NULL;
    
    // Create module
    m = PyModule_Create(&mymodule);
    if (m == NULL)
        return NULL;
    
    // Add type to module
    Py_INCREF(&MyType);
    if (PyModule_AddObject(m, "MyClass", (PyObject *)&MyType) < 0) {
        Py_DECREF(&MyType);
        Py_DECREF(m);
        return NULL;
    }
    
    // Add constants
    PyModule_AddIntConstant(m, "VERSION", 1);
    PyModule_AddStringConstant(m, "AUTHOR", "Name");
    
    return m;
}
```

## ⚠️ Error Handling

### Raise Exceptions
```c
// Standard exceptions
PyErr_SetString(PyExc_TypeError, "Wrong type");
PyErr_SetString(PyExc_ValueError, "Invalid value");
PyErr_SetString(PyExc_RuntimeError, "Something went wrong");
PyErr_SetString(PyExc_MemoryError, "Out of memory");

// Formatted error message
PyErr_Format(PyExc_ValueError, "Expected positive, got %d", value);

// Check if error occurred
if (PyErr_Occurred()) {
    // An exception is set
}

// Clear error
PyErr_Clear();
```

### Return Values on Error
```c
// For functions returning PyObject*
return NULL;

// For functions returning int
return -1;

// For functions returning Py_ssize_t
return -1;
```

## 🔒 Reference Counting

```c
// Increment reference count
Py_INCREF(obj);
Py_XINCREF(obj);  // NULL-safe version

// Decrement reference count
Py_DECREF(obj);
Py_XDECREF(obj);  // NULL-safe version

// Clear and decrement
Py_CLEAR(obj);  // Sets obj to NULL after decref

// Return with proper refcount
return obj;  // Caller gets NEW reference
Py_RETURN_NONE;  // Returns None with proper refcount
```

### Reference Rules
- **NEW**: You own it, must DECREF
- **BORROWED**: You don't own it, don't DECREF
- **STOLEN**: Function takes ownership

### Functions That Return NEW References
- `PyLong_FromLong()`
- `PyUnicode_FromString()`
- `PyList_New()`
- `PyDict_New()`
- Most creation functions

### Functions That Return BORROWED References
- `PyTuple_GetItem()`
- `PyList_GetItem()`
- `PyDict_GetItem()`

### Functions That STEAL References
- `PyList_SetItem()`
- `PyTuple_SetItem()`
- `PyModule_AddObject()` (on success)

## 🔍 Type Checking

```c
// Check specific type
if (PyLong_Check(obj)) { }
if (PyFloat_Check(obj)) { }
if (PyUnicode_Check(obj)) { }
if (PyList_Check(obj)) { }
if (PyDict_Check(obj)) { }
if (PyTuple_Check(obj)) { }

// Check against custom type
if (PyObject_TypeCheck(obj, &MyType)) { }

// Check if callable
if (PyCallable_Check(obj)) { }
```

## 🛠️ Common Patterns

### Iterate List
```c
Py_ssize_t size = PyList_Size(list);
for (Py_ssize_t i = 0; i < size; i++) {
    PyObject *item = PyList_GetItem(list, i);  // Borrowed ref
    // Use item
}
```

### Iterate Dict
```c
PyObject *key, *value;
Py_ssize_t pos = 0;
while (PyDict_Next(dict, &pos, &key, &value)) {
    // key and value are borrowed references
}
```

### Call Python Function
```c
PyObject *result = PyObject_CallFunction(func, "si", "hello", 42);
if (result == NULL) {
    // Handle error
}
Py_XDECREF(result);
```

### Import Module
```c
PyObject *module = PyImport_ImportModule("os.path");
if (module == NULL) {
    return NULL;
}
Py_DECREF(module);
```

## 📝 Build Configuration (setup.py)

```python
from setuptools import setup, Extension

module = Extension(
    'mymodule',
    sources=['mymodule.c'],
    extra_compile_args=['-std=c11'],
    include_dirs=['/path/to/headers'],
    library_dirs=['/path/to/libs'],
    libraries=['mylib'],
)

setup(
    name='mymodule',
    version='1.0',
    ext_modules=[module],
)
```

## 🚀 Build Commands

```bash
# Development build
python3 setup.py build_ext --inplace

# Install
python3 setup.py install --user

# Clean
rm -rf build/ *.so *.egg-info
```

## 🐛 Debugging

```bash
# GDB
gdb python3
(gdb) run script.py
(gdb) bt  # backtrace

# Valgrind (memory leaks)
valgrind --leak-check=full python3 script.py

# Print statements
printf("Debug: value = %d\n", value);
fflush(stdout);
```
