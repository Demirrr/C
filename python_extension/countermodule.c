/*
 * ============================================================================
 * Python C Extension: Counter Class
 * ============================================================================
 * 
 * This module demonstrates how to create Python classes in C using the
 * Python/C API. It implements a simple Counter class with increment/decrement
 * operations.
 * 
 * LEARNING OBJECTIVES:
 * - Understand Python object creation lifecycle (__new__ and __init__)
 * - Implement methods callable from Python
 * - Expose C struct members as Python attributes
 * - Handle reference counting properly
 * - Parse arguments from Python
 * 
 * PYTHON EQUIVALENT:
 * 
 *     class Counter:
 *         def __init__(self, initial_value=0, step=1):
 *             self.count = initial_value
 *             self.step = step
 *         
 *         def increment(self):
 *             self.count += self.step
 *         
 *         def decrement(self):
 *             self.count -= self.step
 *         
 *         def get_count(self):
 *             return self.count
 * 
 * DOCUMENTATION:
 * https://docs.python.org/3/extending/extending.html
 * https://docs.python.org/3/extending/newtypes.html
 * https://docs.python.org/3/c-api/index.html
 * 
 * ============================================================================
 */

#define PY_SSIZE_T_CLEAN  // Use Py_ssize_t for size parameters
#include <Python.h>
#include "structmember.h"  // For PyMemberDef (direct attribute access)

/*
 * ============================================================================
 * SECTION 1: TYPE DEFINITION
 * ============================================================================
 */

/**
 * CounterObject - C structure representing a Python Counter instance
 * 
 * This structure defines the memory layout of our Python object. Python sees
 * this as an opaque PyObject*, but we know the actual structure.
 * 
 * MEMORY LAYOUT:
 * +------------------+
 * | PyObject_HEAD    |  <- Python's object header (refcount, type pointer)
 * +------------------+
 * | int count        |  <- Our data: current count value
 * +------------------+
 * | int step         |  <- Our data: increment/decrement step
 * +------------------+
 * 
 * PyObject_HEAD MUST be the first member - Python relies on this!
 */
typedef struct {
    PyObject_HEAD     // Required Python object header
    int count;        // Current counter value
    int step;         // Step size for increment/decrement
} CounterObject;

/*
 * ============================================================================
 * SECTION 2: OBJECT LIFECYCLE FUNCTIONS
 * ============================================================================
 * 
 * Python objects have a two-stage creation process:
 * 1. __new__  (Counter_new)  - Allocates memory
 * 2. __init__ (Counter_init) - Initializes values
 * 
 * And one destruction stage:
 * 3. __del__  (Counter_dealloc) - Frees resources
 */

/**
 * Counter_dealloc - Destructor for Counter objects
 * 
 * Called automatically when Python's reference count reaches zero.
 * This is similar to Python's __del__, but is guaranteed to be called.
 * 
 * RESPONSIBILITIES:
 * - Free any dynamically allocated memory
 * - Decrement reference counts of owned Python objects
 * - Call tp_free to deallocate the object itself
 * 
 * @param self: The Counter object being destroyed
 */
static void
Counter_dealloc(CounterObject *self)
{
    /*
     * Use the type's tp_free to deallocate memory.
     * This is better than calling free() directly because:
     * - It respects Python's memory allocator
     * - It works correctly with subclasses
     */
    Py_TYPE(self)->tp_free((PyObject *) self);
    
    /*
     * IMPORTANT: If this object held references to other Python objects,
     * we would need to release them here:
     * 
     *     Py_XDECREF(self->some_python_object);
     * 
     * Py_XDECREF is NULL-safe, so we can use it even if the pointer might be NULL.
     */
}

/**
 * Counter_new - Allocator for Counter objects (__new__)
 * 
 * This is the first stage of Python object creation. It allocates memory
 * for the object but doesn't initialize it with user values yet.
 * 
 * ANALOGY: Think of this like buying an empty house - the structure exists,
 * but it's not furnished yet. __init__ does the furnishing.
 * 
 * WHEN IS THIS CALLED?
 * - Every time a new Counter instance is created: Counter(10, 5)
 * - Before __init__ is called
 * 
 * RETURN VALUE:
 * - New reference to the allocated object (caller owns it)
 * - NULL on allocation failure
 * 
 * @param type: The type object (CounterType)
 * @param args: Positional arguments (unused here, handled in __init__)
 * @param kwds: Keyword arguments (unused here, handled in __init__)
 */
static PyObject *
Counter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    CounterObject *self;
    
    /*
     * Allocate memory for the object.
     * type->tp_alloc is a function pointer that handles allocation.
     * The '0' parameter is for variable-sized objects (like lists or strings).
     * We don't need it since Counter is fixed-size.
     */
    self = (CounterObject *) type->tp_alloc(type, 0);
    
    if (self != NULL) {
        /*
         * Initialize to safe default values.
         * This is important because __init__ might not be called!
         * (e.g., when unpickling or copying objects)
         */
        self->count = 0;
        self->step = 1;
    }
    
    /* Return the new object (or NULL if allocation failed) */
    return (PyObject *) self;
}

/**
 * Counter_init - Initializer for Counter objects (__init__)
 * 
 * This is the second stage of object creation. It initializes the allocated
 * object with user-provided values.
 * 
 * PYTHON SIGNATURE:
 *     def __init__(self, initial_value=0, step=1):
 * 
 * ARGUMENT PARSING:
 * Format string "|ii" means:
 *   |  = All following arguments are optional
 *   i  = First argument: integer (initial_value)
 *   i  = Second argument: integer (step)
 * 
 * RETURN VALUE:
 * - 0 on success
 * - -1 on error (Python exception must be set)
 * 
 * @param self: The Counter object to initialize
 * @param args: Positional arguments tuple from Python
 * @param kwds: Keyword arguments dict from Python
 */
static int
Counter_init(CounterObject *self, PyObject *args, PyObject *kwds)
{
    /* Keyword argument names - must be NULL-terminated */
    static char *kwlist[] = {"initial_value", "step", NULL};
    
    /* Default values for optional arguments */
    int initial_value = 0;
    int step = 1;

    /*
     * Parse Python arguments into C variables.
     * 
     * PyArg_ParseTupleAndKeywords does several things:
     * 1. Validates argument count and types
     * 2. Converts Python objects to C types
     * 3. Sets a Python exception on error
     * 4. Returns false on failure
     */
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ii", kwlist,
                                     &initial_value, &step)) {
        return -1;  /* Error - exception already set */
    }

    /* Initialize object members with parsed values */
    self->count = initial_value;
    self->step = step;

    printf("[C Extension] Counter initialized: count=%d, step=%d\n",
           self->count, self->step);

    return 0;  /* Success */
}

/*
 * ============================================================================
 * SECTION 3: COUNTER METHODS
 * ============================================================================
 * 
 * These functions implement the Counter class methods callable from Python.
 * 
 * METHOD SIGNATURE REQUIREMENTS:
 * - Must return PyObject* (even for void methods - return None)
 * - First parameter is always 'self'
 * - Method types:
 *   METH_NOARGS:   func(self, PyObject *Py_UNUSED(ignored))
 *   METH_VARARGS:  func(self, PyObject *args)
 *   METH_KEYWORDS: func(self, PyObject *args, PyObject *kwargs)
 */

/**
 * Counter_increment - Increment counter by step value
 * 
 * PYTHON EQUIVALENT:
 *     def increment(self):
 *         self.count += self.step
 * 
 * @param self: Counter instance
 * @param ignored: Unused (required by METH_NOARGS signature)
 * @return: Py_None
 */
static PyObject *
Counter_increment(CounterObject *self, PyObject *Py_UNUSED(ignored))
{
    self->count += self->step;
    
    /*
     * Py_RETURN_NONE is a convenience macro that:
     * 1. Increments the reference count of None (Py_INCREF)
     * 2. Returns None
     * 
     * This is the C equivalent of 'return None' in Python.
     */
    Py_RETURN_NONE;
}

/**
 * Counter_decrement - Decrement counter by step value
 * 
 * PYTHON EQUIVALENT:
 *     def decrement(self):
 *         self.count -= self.step
 */
static PyObject *
Counter_decrement(CounterObject *self, PyObject *Py_UNUSED(ignored))
{
    self->count -= self->step;
    Py_RETURN_NONE;
}

/**
 * Counter_get_count - Get current count value
 * 
 * PYTHON EQUIVALENT:
 *     def get_count(self):
 *         return self.count
 * 
 * DEMONSTRATES: Converting C types to Python objects
 * 
 * @return: New reference to Python integer object
 */
static PyObject *
Counter_get_count(CounterObject *self, PyObject *Py_UNUSED(ignored))
{
    /*
     * PyLong_FromLong converts a C long to a Python integer.
     * Returns a NEW reference - caller is responsible for it.
     * 
     * Other conversion functions:
     * - PyFloat_FromDouble() for floats
     * - PyUnicode_FromString() for strings
     * - PyBool_FromLong() for booleans
     */
    return PyLong_FromLong(self->count);
}

/**
 * Counter_reset - Reset counter to zero
 * 
 * PYTHON EQUIVALENT:
 *     def reset(self):
 *         self.count = 0
 */
static PyObject *
Counter_reset(CounterObject *self, PyObject *Py_UNUSED(ignored))
{
    self->count = 0;
    Py_RETURN_NONE;
}

/**
 * Counter_set_step - Set the step value
 * 
 * PYTHON EQUIVALENT:
 *     def set_step(self, new_step):
 *         self.step = new_step
 * 
 * DEMONSTRATES: Parsing method arguments from Python
 * 
 * @param args: Tuple of positional arguments from Python
 */
static PyObject *
Counter_set_step(CounterObject *self, PyObject *args)
{
    int new_step;
    
    /*
     * Parse arguments with PyArg_ParseTuple.
     * Format string "i" means: one required integer argument.
     * 
     * Common format codes:
     * - "i"   : int
     * - "d"   : double
     * - "s"   : string (char*)
     * - "O"   : any Python object (PyObject*)
     * - "|i"  : optional int
     * - "ii"  : two required ints
     */
    if (!PyArg_ParseTuple(args, "i", &new_step)) {
        return NULL;  /* Error - exception already set by PyArg_ParseTuple */
    }
    
    /* Update state */
    self->step = new_step;
    
    Py_RETURN_NONE;
}

/*
 * ============================================================================
 * SECTION 4: METHOD AND MEMBER TABLES
 * ============================================================================
 * 
 * These tables tell Python about the class's methods and attributes.
 */

/**
 * Counter_methods - Method table for Counter class
 * 
 * This array maps Python method names to C function pointers.
 * Python uses this table to look up and call methods.
 * 
 * Each entry has the format:
 *     {"python_name", C_function, calling_convention, "docstring"}
 * 
 * CALLING CONVENTIONS:
 * - METH_NOARGS:    Method takes no arguments (except self)
 * - METH_VARARGS:   Method takes positional arguments
 * - METH_KEYWORDS:  Method takes keyword arguments
 * - METH_O:         Method takes exactly one object argument
 * - METH_STATIC:    Static method (no self)
 * - METH_CLASS:     Class method (receives class, not instance)
 */
static PyMethodDef Counter_methods[] = {
    {
        "increment",
        (PyCFunction) Counter_increment,
        METH_NOARGS,
        "increment()\n"
        "--\n\n"
        "Increment the counter by the step value.\n\n"
        "Example:\n"
        "    >>> c = Counter(5, 2)\n"
        "    >>> c.increment()\n"
        "    >>> c.get_count()\n"
        "    7"
    },
    {
        "decrement",
        (PyCFunction) Counter_decrement,
        METH_NOARGS,
        "decrement()\n"
        "--\n\n"
        "Decrement the counter by the step value."
    },
    {
        "get_count",
        (PyCFunction) Counter_get_count,
        METH_NOARGS,
        "get_count()\n"
        "--\n\n"
        "Return the current count value.\n\n"
        "Returns:\n"
        "    int: The current count"
    },
    {
        "reset",
        (PyCFunction) Counter_reset,
        METH_NOARGS,
        "reset()\n"
        "--\n\n"
        "Reset the counter to zero."
    },
    {
        "set_step",
        (PyCFunction) Counter_set_step,
        METH_VARARGS,
        "set_step(new_step)\n"
        "--\n\n"
        "Set the step value for increment/decrement operations.\n\n"
        "Parameters:\n"
        "    new_step (int): The new step value"
    },
    {NULL, NULL, 0, NULL}  /* Sentinel - marks end of array */
};

/**
 * Counter_members - Member table for direct attribute access
 * 
 * This table allows Python code to directly read/write C struct members.
 * 
 * ENABLES SYNTAX:
 *     counter.count      # Direct access (instead of counter.get_count())
 *     counter.step = 5   # Direct assignment
 * 
 * Each entry has the format:
 *     {"name", type, offset, flags, "docstring"}
 * 
 * MEMBER TYPES:
 * - T_INT:    C int
 * - T_LONG:   C long
 * - T_DOUBLE: C double
 * - T_STRING: C string (char*)
 * - T_OBJECT: Python object (PyObject*)
 * 
 * FLAGS:
 * - 0:        Read-write
 * - READONLY: Read-only from Python
 * 
 * offsetof() is a C macro that calculates the byte offset of a member
 * within a struct. Python uses this to find the member in memory.
 */
static PyMemberDef Counter_members[] = {
    {
        "count",
        T_INT,
        offsetof(CounterObject, count),
        0,  /* Read-write */
        "Current count value (read/write)"
    },
    {
        "step",
        T_INT,
        offsetof(CounterObject, step),
        0,  /* Read-write */
        "Step value for increment/decrement operations (read/write)"
    },
    {NULL, 0, 0, 0, NULL}  /* Sentinel */
};

/*
 * ============================================================================
 * SECTION 5: TYPE OBJECT DEFINITION
 * ============================================================================
 * 
 * The PyTypeObject is the "metaclass" - it defines everything about our
 * Counter type. This is analogous to defining a class in Python.
 */

/**
 * CounterType - Type object for Counter class
 * 
 * This massive struct tells Python everything about the Counter type:
 * - How to allocate and free memory
 * - What methods and attributes it has
 * - How to print, compare, hash it
 * - Whether it can be subclassed
 * - And much more...
 * 
 * We only fill in the slots we need. Python fills in the rest with
 * reasonable defaults when we call PyType_Ready().
 * 
 * IMPORTANT SLOTS:
 * - tp_name:     Fully qualified type name
 * - tp_basicsize: Size of our struct in bytes
 * - tp_new:      Memory allocator
 * - tp_init:     Initializer
 * - tp_dealloc:  Destructor
 * - tp_methods:  Method table
 * - tp_members:  Attribute table
 * 
 * OTHER AVAILABLE SLOTS (examples):
 * - tp_repr:     __repr__ implementation
 * - tp_str:      __str__ implementation
 * - tp_hash:     __hash__ implementation
 * - tp_richcompare: Comparison operators
 * - tp_iter:     Iterator protocol
 * - tp_getattro: Attribute access
 */
static PyTypeObject CounterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    
    .tp_name = "countermodule.Counter",
    .tp_doc = PyDoc_STR(
        "Counter(initial_value=0, step=1)\n"
        "--\n\n"
        "A simple counter with increment/decrement operations.\n\n"
        "This class demonstrates Python C extension basics:\n"
        "- Object lifecycle (__new__, __init__, __del__)\n"
        "- Method definitions\n"
        "- Direct attribute access\n"
        "- Argument parsing\n\n"
        "Parameters:\n"
        "    initial_value (int, optional): Starting count value. Default: 0\n"
        "    step (int, optional): Step size for operations. Default: 1\n\n"
        "Example:\n"
        "    >>> c = Counter(10, 5)\n"
        "    >>> c.increment()\n"
        "    >>> c.get_count()\n"
        "    15"
    ),
    .tp_basicsize = sizeof(CounterObject),
    .tp_itemsize = 0,
    
    /* Flags */
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        /* Py_TPFLAGS_DEFAULT: Standard flags for all types */
        /* Py_TPFLAGS_BASETYPE: Allow subclassing in Python */
    
    /* Lifecycle functions */
    .tp_new = Counter_new,
    .tp_init = (initproc) Counter_init,
    .tp_dealloc = (destructor) Counter_dealloc,
    
    /* Method and member tables */
    .tp_methods = Counter_methods,
    .tp_members = Counter_members,
};

/*
 * ============================================================================
 * SECTION 6: MODULE DEFINITION
 * ============================================================================
 * 
 * The module is the top-level container. It holds types, functions, and
 * constants. This is what gets imported in Python: import countermodule
 */

/**
 * countermodule - Module definition
 * 
 * This struct describes the module itself (the .so/.pyd file).
 * A module can contain:
 * - Multiple classes (types)
 * - Module-level functions
 * - Constants
 * - Sub-modules
 * 
 * FIELDS:
 * - m_name: Module name (must match setup.py and PyInit_* function)
 * - m_doc:  Module docstring (appears in help())
 * - m_size: Module state size (-1 for stateless modules)
 * - m_methods: Array of module-level functions (we don't have any)
 */
static PyModuleDef countermodule = {
    PyModuleDef_HEAD_INIT,
    
    .m_name = "countermodule",
    .m_doc = PyDoc_STR(
        "Python C Extension demonstrating object-oriented programming in C.\n\n"
        "This module provides the Counter class, which showcases:\n"
        "- Type definition and object lifecycle\n"
        "- Method implementation\n"
        "- Attribute access\n"
        "- Argument parsing\n"
        "- Reference counting\n\n"
        "Usage:\n"
        "    import countermodule\n"
        "    counter = countermodule.Counter(0, 1)\n"
        "    counter.increment()\n"
    ),
    .m_size = -1,  /* Stateless module */
    
    /*
     * m_methods would go here if we had module-level functions.
     * Example:
     *     static PyMethodDef module_methods[] = {
     *         {"function_name", function_ptr, METH_VARARGS, "docstring"},
     *         {NULL, NULL, 0, NULL}
     *     };
     *     .m_methods = module_methods,
     */
};

/*
 * ============================================================================
 * SECTION 7: MODULE INITIALIZATION
 * ============================================================================
 * 
 * Python calls PyInit_<modulename> when the module is first imported.
 * This is where we set everything up.
 */

/**
 * PyInit_countermodule - Module initialization function
 * 
 * CRITICAL: This function MUST be named PyInit_<modulename> where
 * <modulename> matches both:
 * - The module name in PyModuleDef
 * - The extension name in setup.py
 * 
 * RESPONSIBILITIES:
 * 1. Call PyType_Ready() on all types
 * 2. Create the module object
 * 3. Add types to the module
 * 4. Add constants, functions, etc. to the module
 * 
 * REFERENCE COUNTING:
 * This is crucial! Every PyObject* has a reference count.
 * - NEW reference: You own it, must eventually Py_DECREF it
 * - BORROWED reference: You don't own it, don't Py_DECREF it
 * - STOLEN reference: You give ownership away
 * 
 * @return: New reference to initialized module (or NULL on error)
 */
PyMODINIT_FUNC
PyInit_countermodule(void)
{
    PyObject *m;
    
    /*
     * Step 1: Finalize the type object.
     * 
     * PyType_Ready() does several things:
     * - Fills in missing type slots with defaults
     * - Sets up inheritance from base types
     * - Initializes the type's method cache
     * - Validates the type definition
     * 
     * This MUST be called before using the type!
     */
    if (PyType_Ready(&CounterType) < 0) {
        return NULL;
    }

    /*
     * Step 2: Create the module object.
     * 
     * PyModule_Create() returns a NEW reference.
     */
    m = PyModule_Create(&countermodule);
    if (m == NULL) {
        return NULL;
    }

    /*
     * Step 3: Add the Counter type to the module.
     * 
     * We need to increase the type's reference count because
     * the module will hold a reference to it.
     */
    Py_INCREF(&CounterType);
    
    /*
     * PyModule_AddObject() adds an object to the module's dict.
     * After this, Python code can access: countermodule.Counter
     * 
     * IMPORTANT: PyModule_AddObject() STEALS a reference on success!
     * That's why we Py_INCREF'd above.
     */
    if (PyModule_AddObject(m, "Counter", (PyObject *) &CounterType) < 0) {
        /* AddObject failed - clean up */
        Py_DECREF(&CounterType);  /* Remove the reference we added */
        Py_DECREF(m);             /* Module won't be used */
        return NULL;
    }

    /*
     * Step 4: Add any module-level constants or functions here.
     * Examples:
     *     PyModule_AddIntConstant(m, "VERSION", 1);
     *     PyModule_AddStringConstant(m, "AUTHOR", "Your Name");
     */

    /*
     * Return the fully initialized module.
     * Python takes ownership of this reference.
     */
    return m;
}
