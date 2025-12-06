/*
 * ============================================================================
 * Python C++ Extension: ThreadSafeCounter
 * ============================================================================
 * 
 * This module demonstrates multithreading in C++ Python extensions.
 * It shows both unsafe and safe concurrent operations, teaching:
 * - Race conditions and why they occur
 * - Thread safety with std::atomic and std::mutex
 * - Parallel accumulation for performance
 * - Performance benchmarking (single vs multi-threaded)
 * 
 * EVOLUTION FROM C VERSION:
 * The original Counter class (countermodule.c) was single-threaded.
 * This C++ version adds concurrent operations to demonstrate:
 * 1. The problems that arise with concurrent access
 * 2. How to fix them properly
 * 3. Real performance benefits of parallelism
 * 
 * KEY CONCEPTS DEMONSTRATED:
 * - Race conditions (intentionally shown in unsafe mode)
 * - std::atomic for lock-free thread safety
 * - std::mutex for critical sections
 * - std::thread for parallel execution
 * - Thread pools and work distribution
 * 
 * ============================================================================
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

// C++ headers
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <numeric>
#include <algorithm>

/*
 * ============================================================================
 * SECTION 1: TYPE DEFINITION
 * ============================================================================
 */

/**
 * ThreadSafeCounter - C++ object exposed to Python
 * 
 * Uses std::atomic for thread-safe counter operations without locks.
 * This is much faster than using mutex for simple increment operations.
 */
typedef struct {
    PyObject_HEAD
    std::atomic<long>* count;      // Atomic for thread-safe operations
    int step;                      // Step size for increment/decrement
    std::mutex* mutex;             // Mutex for operations needing serialization
} ThreadSafeCounterObject;

/*
 * ============================================================================
 * SECTION 2: OBJECT LIFECYCLE
 * ============================================================================
 */

/**
 * ThreadSafeCounter_new - Allocator
 */
static PyObject*
ThreadSafeCounter_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    ThreadSafeCounterObject* self;
    self = (ThreadSafeCounterObject*)type->tp_alloc(type, 0);
    
    if (self != NULL) {
        // Initialize C++ objects using placement new
        self->count = new std::atomic<long>(0);
        self->step = 1;
        self->mutex = new std::mutex();
    }
    
    return (PyObject*)self;
}

/**
 * ThreadSafeCounter_init - Initializer
 */
static int
ThreadSafeCounter_init(ThreadSafeCounterObject* self, PyObject* args, PyObject* kwds)
{
    static char* kwlist[] = {(char*)"initial_value", (char*)"step", NULL};
    long initial_value = 0;
    int step = 1;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|li", kwlist,
                                     &initial_value, &step)) {
        return -1;
    }
    
    self->count->store(initial_value);
    self->step = step;
    
    return 0;
}

/**
 * ThreadSafeCounter_dealloc - Destructor
 * 
 * IMPORTANT: Must delete C++ objects explicitly
 */
static void
ThreadSafeCounter_dealloc(ThreadSafeCounterObject* self)
{
    // Delete C++ objects to prevent memory leaks
    delete self->count;
    delete self->mutex;
    
    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
 * ============================================================================
 * SECTION 3: HELPER FUNCTIONS FOR PARALLEL OPERATIONS
 * ============================================================================
 */

/**
 * Worker function for unsafe increment (demonstrates race condition)
 * 
 * This intentionally uses non-atomic operations to show what goes wrong
 * when multiple threads access shared data without synchronization.
 */
static void increment_worker_unsafe(long* count, int step, int iterations)
{
    for (int i = 0; i < iterations; ++i) {
        // RACE CONDITION: This is NOT atomic!
        // Multiple threads reading and writing cause lost updates
        long temp = *count;
        // Small delay to increase probability of race condition
        for (volatile int j = 0; j < 10; ++j) {}
        temp += step;
        *count = temp;
    }
}

/**
 * Worker function for safe increment using atomic operations
 * 
 * std::atomic ensures operations are atomic at the CPU level.
 */
static void increment_worker_safe(std::atomic<long>* count, int step, int iterations)
{
    for (int i = 0; i < iterations; ++i) {
        // This is atomic - no race condition!
        count->fetch_add(step, std::memory_order_relaxed);
    }
}

/**
 * Worker function for parallel accumulation
 * 
 * Each thread sums a portion of the array, then we combine results.
 */
static long accumulate_worker(const long* data, size_t start, size_t end)
{
    long sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += data[i];
    }
    return sum;
}

/*
 * ============================================================================
 * SECTION 4: COUNTER METHODS - THREAD SAFETY DEMONSTRATION
 * ============================================================================
 */

/**
 * increment_unsafe_demo - Demonstrates race condition
 * 
 * INTENTIONALLY UNSAFE! This shows what happens when threads
 * access shared data without synchronization.
 * 
 * Python signature: increment_unsafe_demo(iterations, num_threads)
 */
static PyObject*
ThreadSafeCounter_increment_unsafe_demo(ThreadSafeCounterObject* self, PyObject* args)
{
    int iterations, num_threads;
    
    if (!PyArg_ParseTuple(args, "ii", &iterations, &num_threads)) {
        return NULL;
    }
    
    // Save initial value
    long initial = self->count->load();
    
    // Create a regular long for unsafe access
    long unsafe_count = initial;
    
    // Launch threads that will create race conditions
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment_worker_unsafe, &unsafe_count, 
                           self->step, iterations);
    }
    
    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Expected vs actual
    long expected = initial + (static_cast<long>(self->step) * iterations * num_threads);
    long actual = unsafe_count;
    
    // Return (expected, actual, time_us, lost_updates)
    return Py_BuildValue("(llLl)", expected, actual, duration.count(), expected - actual);
}

/**
 * increment_safe - Thread-safe increment using atomic operations
 * 
 * This demonstrates the correct way to handle concurrent increments.
 * 
 * Python signature: increment_safe(iterations, num_threads)
 */
static PyObject*
ThreadSafeCounter_increment_safe(ThreadSafeCounterObject* self, PyObject* args)
{
    int iterations, num_threads;
    
    if (!PyArg_ParseTuple(args, "ii", &iterations, &num_threads)) {
        return NULL;
    }
    
    long initial = self->count->load();
    
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Launch threads with atomic operations
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment_worker_safe, self->count, 
                           self->step, iterations);
    }
    
    // Wait for completion
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    long expected = initial + (static_cast<long>(self->step) * iterations * num_threads);
    long actual = self->count->load();
    
    // Return (expected, actual, time_us, lost_updates)
    return Py_BuildValue("(llLl)", expected, actual, duration.count(), 0);
}

/**
 * accumulate_parallel - Parallel sum of array
 * 
 * Demonstrates "embarrassingly parallel" problem where each thread
 * works on independent data, then results are combined.
 * 
 * Python signature: accumulate_parallel(list, num_threads)
 */
static PyObject*
ThreadSafeCounter_accumulate_parallel(ThreadSafeCounterObject* self, PyObject* args)
{
    PyObject* py_list;
    int num_threads;
    
    if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &py_list, &num_threads)) {
        return NULL;
    }
    
    Py_ssize_t size = PyList_Size(py_list);
    if (size == 0) {
        return PyLong_FromLong(0);
    }
    
    // Convert Python list to C++ vector
    std::vector<long> data(size);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyList_GetItem(py_list, i);
        if (!PyLong_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "List must contain integers");
            return NULL;
        }
        data[i] = PyLong_AsLong(item);
    }
    
    // Parallel accumulation
    num_threads = std::min(num_threads, static_cast<int>(size));
    size_t chunk_size = size / num_threads;
    
    std::vector<std::thread> threads;
    std::vector<long> partial_sums(num_threads);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        size_t start_idx = i * chunk_size;
        size_t end_idx = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        
        threads.emplace_back([&data, &partial_sums, i, start_idx, end_idx]() {
            partial_sums[i] = accumulate_worker(data.data(), start_idx, end_idx);
        });
    }
    
    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto parallel_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Combine results
    long total = std::accumulate(partial_sums.begin(), partial_sums.end(), 0L);
    
    // Sequential version for comparison
    start = std::chrono::high_resolution_clock::now();
    long sequential_sum = std::accumulate(data.begin(), data.end(), 0L);
    end = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double speedup = static_cast<double>(sequential_time.count()) / parallel_time.count();
    
    // Return (sum, parallel_time_us, sequential_time_us, speedup)
    return Py_BuildValue("(lLLd)", total, parallel_time.count(), 
                        sequential_time.count(), speedup);
}

/**
 * get_count - Thread-safe read
 */
static PyObject*
ThreadSafeCounter_get_count(ThreadSafeCounterObject* self, PyObject* Py_UNUSED(ignored))
{
    return PyLong_FromLong(self->count->load());
}

/**
 * reset - Thread-safe reset
 */
static PyObject*
ThreadSafeCounter_reset(ThreadSafeCounterObject* self, PyObject* Py_UNUSED(ignored))
{
    self->count->store(0);
    Py_RETURN_NONE;
}

/**
 * set_step - Set step value
 */
static PyObject*
ThreadSafeCounter_set_step(ThreadSafeCounterObject* self, PyObject* args)
{
    int new_step;
    
    if (!PyArg_ParseTuple(args, "i", &new_step)) {
        return NULL;
    }
    
    self->step = new_step;
    Py_RETURN_NONE;
}

/*
 * ============================================================================
 * SECTION 5: METHOD TABLE
 * ============================================================================
 */

static PyMethodDef ThreadSafeCounter_methods[] = {
    {
        "increment_unsafe_demo",
        (PyCFunction)ThreadSafeCounter_increment_unsafe_demo,
        METH_VARARGS,
        "increment_unsafe_demo(iterations, num_threads)\n"
        "--\n\n"
        "Demonstrate race condition with unsafe concurrent increments.\n"
        "INTENTIONALLY UNSAFE to show what goes wrong!\n\n"
        "Returns: (expected, actual, time_us, lost_updates)"
    },
    {
        "increment_safe",
        (PyCFunction)ThreadSafeCounter_increment_safe,
        METH_VARARGS,
        "increment_safe(iterations, num_threads)\n"
        "--\n\n"
        "Thread-safe increment using atomic operations.\n\n"
        "Returns: (expected, actual, time_us, lost_updates)"
    },
    {
        "accumulate_parallel",
        (PyCFunction)ThreadSafeCounter_accumulate_parallel,
        METH_VARARGS,
        "accumulate_parallel(list, num_threads)\n"
        "--\n\n"
        "Sum array in parallel and compare with sequential version.\n\n"
        "Returns: (sum, parallel_time_us, sequential_time_us, speedup)"
    },
    {
        "get_count",
        (PyCFunction)ThreadSafeCounter_get_count,
        METH_NOARGS,
        "get_count()\n--\n\nGet current count value."
    },
    {
        "reset",
        (PyCFunction)ThreadSafeCounter_reset,
        METH_NOARGS,
        "reset()\n--\n\nReset counter to zero."
    },
    {
        "set_step",
        (PyCFunction)ThreadSafeCounter_set_step,
        METH_VARARGS,
        "set_step(new_step)\n--\n\nSet step value."
    },
    {NULL, NULL, 0, NULL}
};

/*
 * ============================================================================
 * SECTION 6: TYPE OBJECT
 * ============================================================================
 */

static PyTypeObject ThreadSafeCounterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "threadcounter.ThreadSafeCounter",              /* tp_name */
    sizeof(ThreadSafeCounterObject),                /* tp_basicsize */
    0,                                               /* tp_itemsize */
    (destructor)ThreadSafeCounter_dealloc,          /* tp_dealloc */
    0,                                               /* tp_vectorcall_offset */
    0,                                               /* tp_getattr */
    0,                                               /* tp_setattr */
    0,                                               /* tp_as_async */
    0,                                               /* tp_repr */
    0,                                               /* tp_as_number */
    0,                                               /* tp_as_sequence */
    0,                                               /* tp_as_mapping */
    0,                                               /* tp_hash */
    0,                                               /* tp_call */
    0,                                               /* tp_str */
    0,                                               /* tp_getattro */
    0,                                               /* tp_setattro */
    0,                                               /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                              /* tp_flags */
    PyDoc_STR(
        "ThreadSafeCounter(initial_value=0, step=1)\n"
        "--\n\n"
        "A counter demonstrating thread safety in C++ Python extensions.\n\n"
        "This class shows:\n"
        "- Race conditions (unsafe mode)\n"
        "- Thread safety with std::atomic\n"
        "- Parallel performance benefits\n\n"
        "Methods:\n"
        "- increment_unsafe_demo(): Show race condition\n"
        "- increment_safe(): Thread-safe increment\n"
        "- accumulate_parallel(): Parallel array sum"
    ),                                               /* tp_doc */
    0,                                               /* tp_traverse */
    0,                                               /* tp_clear */
    0,                                               /* tp_richcompare */
    0,                                               /* tp_weaklistoffset */
    0,                                               /* tp_iter */
    0,                                               /* tp_iternext */
    ThreadSafeCounter_methods,                       /* tp_methods */
    0,                                               /* tp_members */
    0,                                               /* tp_getset */
    0,                                               /* tp_base */
    0,                                               /* tp_dict */
    0,                                               /* tp_descr_get */
    0,                                               /* tp_descr_set */
    0,                                               /* tp_dictoffset */
    (initproc)ThreadSafeCounter_init,               /* tp_init */
    0,                                               /* tp_alloc */
    ThreadSafeCounter_new,                          /* tp_new */
};

/*
 * ============================================================================
 * SECTION 7: MODULE DEFINITION AND INITIALIZATION
 * ============================================================================
 */

static PyModuleDef threadcountermodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "threadcounter",
    .m_doc = PyDoc_STR(
        "C++ extension demonstrating multithreading and thread safety.\n\n"
        "This module shows:\n"
        "1. Race conditions and why they occur\n"
        "2. Thread-safe operations with std::atomic\n"
        "3. Performance benefits of parallel computation"
    ),
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_threadcounter(void)
{
    PyObject* m;
    
    if (PyType_Ready(&ThreadSafeCounterType) < 0) {
        return NULL;
    }
    
    m = PyModule_Create(&threadcountermodule);
    if (m == NULL) {
        return NULL;
    }
    
    Py_INCREF(&ThreadSafeCounterType);
    if (PyModule_AddObject(m, "ThreadSafeCounter", 
                          (PyObject*)&ThreadSafeCounterType) < 0) {
        Py_DECREF(&ThreadSafeCounterType);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}
