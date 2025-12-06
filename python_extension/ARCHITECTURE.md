# Project Structure and Architecture

## 📁 File Organization

```
python_c_extension/
│
├── 📘 Documentation
│   ├── README.md              # Project overview and quick start
│   ├── TUTORIAL.md            # Deep dive into concepts
│   ├── QUICK_REFERENCE.md     # Cheat sheet for API calls
│   └── ARCHITECTURE.md        # This file - visual guides
│
├── 💻 Source Code
│   ├── countermodule.c        # Main C extension (heavily commented)
│   ├── test_counter.py        # Comprehensive test suite
│   └── setup.py               # Build configuration
│
├── 🛠️ Build Tools
│   └── Makefile               # Convenient build commands
│
└── 📦 Generated (after build)
    ├── build/                 # Build artifacts
    └── *.so                   # Compiled extension module
```

## 🏗️ Code Architecture

### High-Level View

```
┌─────────────────────────────────────────────────────────────────┐
│                        Python Program                            │
│                                                                   │
│    import countermodule                                          │
│    counter = countermodule.Counter(10, 5)                       │
│    counter.increment()                                           │
│                                                                   │
└───────────────────────────┬─────────────────────────────────────┘
                            │
                            │ Python/C API Bridge
                            │ (transparent to user)
                            │
┌───────────────────────────▼─────────────────────────────────────┐
│                     C Extension Module                           │
│                    (countermodule.c)                             │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 1: Type Definition                             │    │
│  │ - CounterObject struct                                  │    │
│  │ - Data layout in memory                                 │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 2: Object Lifecycle                             │    │
│  │ - Counter_new()      (allocate memory)                  │    │
│  │ - Counter_init()     (initialize values)                │    │
│  │ - Counter_dealloc()  (cleanup)                          │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 3: Methods                                      │    │
│  │ - Counter_increment()                                   │    │
│  │ - Counter_decrement()                                   │    │
│  │ - Counter_get_count()                                   │    │
│  │ - Counter_reset()                                       │    │
│  │ - Counter_set_step()                                    │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 4: Tables                                       │    │
│  │ - Counter_methods[]  (method lookup)                    │    │
│  │ - Counter_members[]  (attribute access)                 │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 5: Type Object                                  │    │
│  │ - CounterType        (class definition)                 │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 6: Module Definition                            │    │
│  │ - countermodule      (module structure)                 │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ Section 7: Module Initialization                        │    │
│  │ - PyInit_countermodule()  (entry point)                 │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

## 🔄 Object Creation Flow

```
Python: counter = Counter(10, 5)
           │
           ▼
┌─────────────────────────────────────────┐
│ Python Interpreter                      │
│ 1. Looks up "Counter" in module         │
│ 2. Calls type's tp_new                  │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_new()                           │
│ - Allocates memory (tp_alloc)           │
│ - Sets default values                   │
│ - Returns PyObject* (refcount = 1)      │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_init()                          │
│ - Parses arguments (10, 5)              │
│ - Initializes self->count = 10          │
│ - Initializes self->step = 5            │
│ - Returns 0 (success)                   │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Python Variable                         │
│ counter = <CounterObject @ 0x...>       │
│ refcount = 1                            │
└─────────────────────────────────────────┘
```

## 🔧 Method Call Flow

```
Python: counter.increment()
           │
           ▼
┌─────────────────────────────────────────┐
│ Python Interpreter                      │
│ 1. Gets type from counter               │
│ 2. Looks up "increment" in tp_methods   │
│ 3. Finds Counter_increment              │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_methods[] Lookup                │
│ {                                       │
│   "increment",                          │
│   (PyCFunction)Counter_increment,       │
│   METH_NOARGS,                          │
│   "docstring"                           │
│ }                                       │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_increment(self, ignored)        │
│ - Accesses self->count                  │
│ - Accesses self->step                   │
│ - Performs: count += step               │
│ - Returns Py_None (refcount++)         │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Python                                  │
│ Returns None                            │
└─────────────────────────────────────────┘
```

## 💾 Memory Layout

### Python Object in Memory

```
CounterObject instance at address 0x7fff12345000:

Offset   Size   Field              Value       Description
─────────────────────────────────────────────────────────────────
+0       8      ob_refcnt          2           Reference count
+8       8      ob_type            0x...       Pointer to CounterType
+16      4      count              15          Our data: count value
+20      4      step               5           Our data: step size
─────────────────────────────────────────────────────────────────
Total: 24 bytes (plus potential padding)

PyObject_HEAD is the first 16 bytes (ob_refcnt + ob_type)
```

## 🔢 Reference Counting Example

```
# Initial creation
counter = Counter(10, 5)     # refcount = 1
    │
    ├─ Counter_new() called
    ├─ tp_alloc() creates object (refcount = 1)
    └─ Counter_init() initializes it

# Passing to function
def show(c):
    print(c.count)

show(counter)                # refcount = 2 (temporary)
                            # refcount = 1 (after return)
    │
    ├─ Function receives borrowed reference
    ├─ No INCREF/DECREF needed
    └─ Temporary increase during call

# Storing in list
items = [counter]           # refcount = 2
    │
    ├─ PyList_SetItem() steals reference
    └─ List now owns a reference

# Deletion
del counter                 # refcount = 1
                            # Still in 'items'
    │
    └─ Name binding removed, DECREF called

del items                   # refcount = 0
    │
    ├─ List destroyed
    ├─ Counter_dealloc() called
    └─ Memory freed
```

## 📊 Type System Hierarchy

```
                    object (PyBaseObject_Type)
                       │
                       │ tp_base (inherits from)
                       │
                    Counter (CounterType)
                       │
                       ├─ tp_new       → Counter_new()
                       ├─ tp_init      → Counter_init()
                       ├─ tp_dealloc   → Counter_dealloc()
                       ├─ tp_methods   → Counter_methods[]
                       ├─ tp_members   → Counter_members[]
                       ├─ tp_name      → "countermodule.Counter"
                       ├─ tp_doc       → "Counter objects..."
                       └─ ... (many other slots)

Inheritance: Counter inherits from object automatically
             PyType_Ready() fills in inherited slots
```

## 🗺️ Module Import Flow

```
Python: import countermodule
           │
           ▼
┌─────────────────────────────────────────┐
│ Python Import System                    │
│ 1. Searches for countermodule.so        │
│ 2. Loads shared library                 │
│ 3. Looks for PyInit_countermodule       │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ PyInit_countermodule()                  │
│ 1. PyType_Ready(&CounterType)           │
│    - Fills in type slots                │
│    - Sets up inheritance                │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ 2. PyModule_Create(&countermodule)      │
│    - Creates module object              │
│    - Sets module name and doc           │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ 3. PyModule_AddObject(m, "Counter", ...)│
│    - Adds Counter to module namespace   │
│    - Increments type's refcount         │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ 4. return m                             │
│    - Returns module to Python           │
│    - Module cached in sys.modules       │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Python Namespace                        │
│ countermodule.Counter = <type>          │
└─────────────────────────────────────────┘
```

## 🎯 Argument Parsing Flow

```
Python: counter.set_step(2)
           │
           ▼
┌─────────────────────────────────────────┐
│ Python builds arguments                 │
│ args = (2,)        # tuple              │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_set_step(self, args)            │
│                                         │
│ int new_step;                           │
│ PyArg_ParseTuple(args, "i", &new_step) │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ PyArg_ParseTuple() internals            │
│ 1. Check args is tuple                  │
│ 2. Check tuple size matches format      │
│ 3. Get first item: PyTuple_GetItem(0)  │
│ 4. Check it's an int: PyLong_Check()   │
│ 5. Convert to C: PyLong_AsLong()       │
│ 6. Store in new_step variable          │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Back in Counter_set_step()              │
│ new_step = 2          # C int           │
│ self->step = new_step                   │
│ Py_RETURN_NONE;                         │
└─────────────────────────────────────────┘
```

## 🔒 Thread Safety Considerations

```
┌─────────────────────────────────────────┐
│ Global Interpreter Lock (GIL)          │
│                                         │
│ Automatically held during:              │
│ - Python → C calls                      │
│ - C → Python API calls                  │
│                                         │
│ For long computations:                  │
│                                         │
│ Py_BEGIN_ALLOW_THREADS                  │
│   // Release GIL                        │
│   // Do expensive C computation         │
│   // No Python API calls allowed!       │
│ Py_END_ALLOW_THREADS                    │
│   // Reacquire GIL                      │
│                                         │
└─────────────────────────────────────────┘
```

## 📈 Build Process

```
Source: countermodule.c
           │
           ▼
┌─────────────────────────────────────────┐
│ gcc -c countermodule.c                  │
│ - Compiles to object file               │
│ - Includes Python headers               │
│ - Creates countermodule.o               │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ gcc -shared countermodule.o             │
│ - Links with Python library             │
│ - Creates shared object (.so)           │
│ - Output: countermodule.cpython-...so   │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Python can import the module            │
│ import countermodule                    │
└─────────────────────────────────────────┘
```

## 🎓 Learning Path Visualization

```
1. BEGINNER
   │
   ├─→ Read README.md
   │   └─→ Understand what the project does
   │
   ├─→ Read countermodule.c Section 1
   │   └─→ Understand CounterObject struct
   │
   └─→ Run tests (test_counter.py)
       └─→ See it work!

2. INTERMEDIATE
   │
   ├─→ Read countermodule.c Sections 2-3
   │   └─→ Understand lifecycle and methods
   │
   ├─→ Read TUTORIAL.md
   │   └─→ Deep dive into concepts
   │
   └─→ Modify code (add new method)
       └─→ Rebuild and test

3. ADVANCED
   │
   ├─→ Read countermodule.c Sections 4-7
   │   └─→ Understand tables and initialization
   │
   ├─→ Study QUICK_REFERENCE.md
   │   └─→ Learn all API patterns
   │
   └─→ Create your own extension
       └─→ Apply learned concepts
```

## 🗃️ Data Flow: Attribute Access

### Direct Member Access (PyMemberDef)

```
Python: value = counter.count
           │
           ▼
┌─────────────────────────────────────────┐
│ Python Attribute Lookup                 │
│ 1. Check instance dict                  │
│ 2. Check type tp_members                │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Counter_members[] Lookup                │
│ {                                       │
│   "count",                              │
│   T_INT,                                │
│   offsetof(CounterObject, count),       │
│   0,                                    │
│   "doc"                                 │
│ }                                       │
└───────────────┬─────────────────────────┘
                │
                ▼
┌─────────────────────────────────────────┐
│ Direct Memory Access                    │
│ 1. Calculate address: self + offset     │
│ 2. Read 4 bytes as int                  │
│ 3. Convert to PyObject: PyLong_FromLong │
│ 4. Return to Python                     │
└─────────────────────────────────────────┘

Result: 15 (Python int)
```

## 🧩 Complete Picture

```
┌─────────────────────────────────────────────────────────────────┐
│                          PYTHON LAYER                            │
│                                                                   │
│  User writes: counter = Counter(10, 5)                          │
│              counter.increment()                                 │
│              print(counter.count)                               │
│                                                                   │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             │ Python/C API
                             │ - Type system
                             │ - Reference counting
                             │ - Argument conversion
                             │
┌────────────────────────────▼────────────────────────────────────┐
│                           C LAYER                                │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ CounterObject: C struct with PyObject_HEAD               │  │
│  │ - Stores Python object metadata                          │  │
│  │ - Stores our data (count, step)                          │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ C Functions: Counter_increment(), Counter_init(), etc.   │  │
│  │ - Manipulate CounterObject                               │  │
│  │ - Return PyObject* to Python                             │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ Type Object: CounterType                                 │  │
│  │ - Defines how Python interacts with Counter             │  │
│  │ - Method table, member table, lifecycle functions        │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

This architecture allows Python code to use C objects naturally while
the C layer provides efficient implementation and direct memory access.
