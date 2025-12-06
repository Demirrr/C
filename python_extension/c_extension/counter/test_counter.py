#!/usr/bin/env python3
"""
Test script for the Counter C extension module

This script demonstrates all features of the Counter class and verifies
that the C extension works correctly. Each test showcases different
aspects of Python/C integration:
- Object creation with arguments
- Method calls
- Direct attribute access
- Reference counting (multiple instances)
- Default values
"""

import sys
import countermodule

def test_counter():
    
    print("=" * 70)
    print("Testing Counter Class - Python/C Extension Demo")
    print("=" * 70)
    print()
    
    # Test 1: Basic creation and methods
    print("Test 1: Object Creation and Method Calls")
    print("-" * 70)
    print("DEMONSTRATES: __init__ with arguments, method calls via Python/C API")
    print()
    print("Creating: counter = Counter(10, 5)")
    counter = countermodule.Counter(10, 5)
    print(f"  Initial count: {counter.get_count()}")
    assert counter.get_count() == 10, "Initial value should be 10"
    
    print("Calling: counter.increment()")
    counter.increment()
    print(f"  After increment: {counter.get_count()}")
    assert counter.get_count() == 15, "After increment should be 15"
    
    print("Calling: counter.increment()")
    counter.increment()
    print(f"  After another increment: {counter.get_count()}")
    assert counter.get_count() == 20, "After second increment should be 20"
    
    print("Calling: counter.decrement()")
    counter.decrement()
    print(f"  After decrement: {counter.get_count()}")
    assert counter.get_count() == 15, "After decrement should be 15"
    print()
    print("  ✓ Test 1 Passed - Methods work correctly!")
    print()
    
    # Test 2: Direct attribute access
    print("Test 2: Direct Attribute Access")
    print("-" * 70)
    print("DEMONSTRATES: PyMemberDef - accessing C struct members from Python")
    print()
    print("Reading attributes directly:")
    print(f"  counter.count = {counter.count}")
    print(f"  counter.step = {counter.step}")
    assert counter.count == 15, "Count attribute should be 15"
    assert counter.step == 5, "Step attribute should be 5"
    print()
    print("Writing attributes directly:")
    print("  counter.count = 100")
    counter.count = 100
    print(f"  New value: {counter.count}")
    assert counter.count == 100, "Direct write should work"
    print()
    print("  ✓ Test 2 Passed - Direct attribute access works!")
    print()
    
    # Test 3: Changing step and reset
    print("Test 3: Method Arguments and State Changes")
    print("-" * 70)
    print("DEMONSTRATES: PyArg_ParseTuple - parsing arguments from Python")
    print()
    counter.count = 15  # Reset from previous test
    print("Calling: counter.set_step(2)")
    counter.set_step(2)
    print(f"  Step changed to: {counter.step}")
    
    print("Calling: counter.increment()")
    counter.increment()
    print(f"  After increment with step=2: {counter.get_count()}")
    assert counter.get_count() == 17, "After increment with step=2 should be 17"
    
    print("Calling: counter.reset()")
    counter.reset()
    print(f"  After reset: {counter.get_count()}")
    assert counter.get_count() == 0, "After reset should be 0"
    print()
    print("  ✓ Test 3 Passed - Stateful operations work!")
    print()
    
    # Test 4: Default values
    print("Test 4: Default Argument Values")
    print("-" * 70)
    print("DEMONSTRATES: Optional arguments with \\|ii\\ format specifier")
    print()
    print("Creating: counter2 = Counter()  # No arguments")
    counter2 = countermodule.Counter()
    print(f"  Default count: {counter2.get_count()}")
    print(f"  Default step: {counter2.step}")
    assert counter2.get_count() == 0, "Default count should be 0"
    assert counter2.step == 1, "Default step should be 1"
    
    print("Calling: counter2.increment()")
    counter2.increment()
    print(f"  After increment: {counter2.get_count()}")
    assert counter2.get_count() == 1, "After increment should be 1"
    print()
    print("  ✓ Test 4 Passed - Default values work!")
    print()
    
    # Test 5: Multiple instances
    print("Test 5: Multiple Independent Instances")
    print("-" * 70)
    print("DEMONSTRATES: Reference counting - each object has independent state")
    print()
    print("Creating: c1 = Counter(0, 1)")
    print("Creating: c2 = Counter(100, 10)")
    c1 = countermodule.Counter(0, 1)
    c2 = countermodule.Counter(100, 10)
    
    print("\nOperating on both:")
    print("  c1.increment()")
    print("  c2.increment()")
    c1.increment()
    c2.increment()
    
    print(f"\nFinal values:")
    print(f"  Counter 1: {c1.get_count()}")
    print(f"  Counter 2: {c2.get_count()}")
    assert c1.get_count() == 1, "Counter 1 should be 1"
    assert c2.get_count() == 110, "Counter 2 should be 110"
    print()
    print("  ✓ Test 5 Passed - Multiple instances work independently!")
    print()
    
    print("=" * 70)
    print("ALL TESTS PASSED! ✓✓✓")
    print("=" * 70)
    print()
    
    # Demo usage
    print("=" * 70)
    print("Demo: Practical Usage Example")
    print("=" * 70)
    print()
    print("Scenario: Counting iterations with step=3")
    print()
    demo = countermodule.Counter(0, 3)
    for i in range(5):
        demo.increment()
        print(f"  Iteration {i+1}: count = {demo.get_count()}")
    
    print()
    print("=" * 70)
    print("Module successfully imported and tested!")
    print("The C extension integrates seamlessly with Python! 🎉")
    print("=" * 70)

if __name__ == "__main__":
    test_counter()
