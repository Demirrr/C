#!/usr/bin/env python3
"""
Test suite for ThreadSafeCounter C++ extension

This demonstrates:
1. Race conditions in concurrent code
2. Thread-safe operations with atomic types
3. Performance benefits of parallel processing
"""

import threadcounter
import sys

def print_header(title):
    print("\n" + "="*70)
    print(title)
    print("="*70)

def print_section(title):
    print("\n" + "-"*70)
    print(title)
    print("-"*70)

def test_race_condition():
    """Demonstrate race condition with unsafe concurrent access"""
    print_header("Test 1: Race Condition Demonstration (UNSAFE)")
    print("DEMONSTRATES: What goes wrong without thread synchronization\n")
    
    counter = threadcounter.ThreadSafeCounter(0, 1)
    
    # Try with increasing thread counts to make race condition more visible
    for num_threads in [2, 4, 8]:
        print(f"\n🔴 Running UNSAFE increment with {num_threads} threads:")
        print(f"   Each thread: 10,000 iterations")
        
        expected, actual, time_us, lost = counter.increment_unsafe_demo(10000, num_threads)
        
        print(f"   Expected result: {expected:,}")
        print(f"   Actual result:   {actual:,}")
        print(f"   Lost updates:    {lost:,} ⚠️")
        print(f"   Time taken:      {time_us:,} μs")
        print(f"   Error rate:      {(lost/expected*100):.2f}%")
        
        counter.reset()
    
    print("\n📝 EXPLANATION:")
    print("   Race condition occurs when multiple threads read-modify-write")
    print("   the same memory location without synchronization.")
    print("   Result: Lost updates and incorrect final value!")
    print("\n  ✓ Test 1 Complete - Race condition demonstrated")

def test_thread_safe():
    """Demonstrate thread-safe operations with atomic types"""
    print_header("Test 2: Thread-Safe Operations (SAFE)")
    print("DEMONSTRATES: Correct synchronization with std::atomic\n")
    
    counter = threadcounter.ThreadSafeCounter(0, 1)
    
    for num_threads in [2, 4, 8]:
        print(f"\n🟢 Running SAFE increment with {num_threads} threads:")
        print(f"   Each thread: 10,000 iterations")
        
        expected, actual, time_us, lost = counter.increment_safe(10000, num_threads)
        
        print(f"   Expected result: {expected:,}")
        print(f"   Actual result:   {actual:,} ✓")
        print(f"   Lost updates:    {lost:,}")
        print(f"   Time taken:      {time_us:,} μs")
        
        assert expected == actual, "Thread-safe version should have no lost updates!"
        counter.reset()
    
    print("\n📝 EXPLANATION:")
    print("   std::atomic provides lock-free thread safety at CPU level.")
    print("   Operations are guaranteed to be atomic - no race conditions!")
    print("\n  ✓ Test 2 Complete - Thread safety verified")

def test_parallel_accumulation():
    """Demonstrate performance benefits of parallel processing"""
    print_header("Test 3: Parallel Performance Benefits")
    print("DEMONSTRATES: Real speedup from multithreading\n")
    
    counter = threadcounter.ThreadSafeCounter()
    
    # Create large array for meaningful parallel work
    print("Creating array with 10,000,000 elements...")
    large_array = list(range(1, 10_000_001))
    expected_sum = sum(large_array)  # Python's built-in for verification
    print(f"Expected sum: {expected_sum:,}\n")
    
    for num_threads in [1, 2, 4, 8]:
        print(f"🔄 Accumulating with {num_threads} thread(s):")
        
        result_sum, parallel_time, sequential_time, speedup = \
            counter.accumulate_parallel(large_array, num_threads)
        
        print(f"   Result:          {result_sum:,} ✓")
        print(f"   Parallel time:   {parallel_time:,} μs")
        print(f"   Sequential time: {sequential_time:,} μs")
        print(f"   Speedup:         {speedup:.2f}x")
        
        if num_threads > 1:
            efficiency = (speedup / num_threads) * 100
            print(f"   Efficiency:      {efficiency:.1f}%")
        
        assert result_sum == expected_sum, "Sum must be correct!"
        print()
    
    print("📝 EXPLANATION:")
    print("   'Embarrassingly parallel' problems like summation scale well.")
    print("   Each thread works on independent data, no synchronization needed.")
    print("   Real speedup depends on: CPU cores, cache, memory bandwidth.")
    print("\n  ✓ Test 3 Complete - Parallel speedup demonstrated")

def test_basic_operations():
    """Test basic counter operations"""
    print_header("Test 4: Basic Operations")
    print("DEMONSTRATES: Standard counter functionality\n")
    
    counter = threadcounter.ThreadSafeCounter(100, 5)
    print(f"Initial count: {counter.get_count()}")
    assert counter.get_count() == 100
    
    counter.set_step(10)
    print(f"Changed step to: 10")
    
    counter.reset()
    print(f"After reset: {counter.get_count()}")
    assert counter.get_count() == 0
    
    print("\n  ✓ Test 4 Complete - Basic operations work")

def test_comparison_unsafe_vs_safe():
    """Direct comparison of unsafe vs safe"""
    print_header("Test 5: Direct Comparison - Unsafe vs Safe")
    print("DEMONSTRATES: Side-by-side comparison\n")
    
    counter = threadcounter.ThreadSafeCounter(0, 1)
    
    num_threads = 8
    iterations = 50000
    
    print(f"Configuration: {num_threads} threads, {iterations:,} iterations each\n")
    
    # Unsafe
    print("🔴 UNSAFE version:")
    expected, actual_unsafe, time_unsafe, lost_unsafe = \
        counter.increment_unsafe_demo(iterations, num_threads)
    print(f"   Expected: {expected:,}")
    print(f"   Actual:   {actual_unsafe:,}")
    print(f"   Lost:     {lost_unsafe:,} ⚠️")
    print(f"   Time:     {time_unsafe:,} μs")
    
    counter.reset()
    
    # Safe
    print("\n🟢 SAFE version:")
    expected, actual_safe, time_safe, lost_safe = \
        counter.increment_safe(iterations, num_threads)
    print(f"   Expected: {expected:,}")
    print(f"   Actual:   {actual_safe:,}")
    print(f"   Lost:     {lost_safe:,}")
    print(f"   Time:     {time_safe:,} μs")
    
    print(f"\n📊 COMPARISON:")
    print(f"   Correctness: Unsafe lost {(lost_unsafe/expected*100):.2f}% of updates")
    print(f"   Performance: Safe is {(time_safe/time_unsafe):.2f}x the time")
    print(f"                (slightly slower due to atomic operations)")
    
    print("\n  ✓ Test 5 Complete - Comparison shown")

def run_all_tests():
    """Run complete test suite"""
    print("="*70)
    print("ThreadSafeCounter C++ Extension - Complete Test Suite")
    print("="*70)
    print("\nThis test suite demonstrates:")
    print("  1. Race conditions and data races")
    print("  2. Thread-safe programming with atomics")
    print("  3. Performance benefits of parallel processing")
    print("  4. Proper synchronization techniques")
    
    try:
        test_race_condition()
        test_thread_safe()
        test_parallel_accumulation()
        test_basic_operations()
        test_comparison_unsafe_vs_safe()
        
        print("\n" + "="*70)
        print("ALL TESTS PASSED! ✓✓✓")
        print("="*70)
        print("\n🎉 You've seen how C++ multithreading works in Python extensions!")
        print("📚 Key takeaways:")
        print("   - Always synchronize shared data access")
        print("   - std::atomic for simple counters (fast, lock-free)")
        print("   - std::mutex for complex critical sections")
        print("   - Parallel processing gives real speedups for the right problems")
        print("="*70)
        
        return 0
        
    except Exception as e:
        print(f"\n❌ TEST FAILED: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == "__main__":
    sys.exit(run_all_tests())
