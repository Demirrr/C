from setuptools import setup, Extension
import sys

# C++ extension with threading support
threadcounter = Extension(
    'threadcounter',
    sources=['threadcounter.cpp'],
    extra_compile_args=[
        '-std=c++11',
        '-pthread',
        '-O3',  # Optimization for better performance benchmarks
    ],
    extra_link_args=['-pthread'],
    language='c++',
)

setup(
    name='threadcounter',
    version='1.0',
    description='C++ Python Extension - Thread-Safe Counter with Multithreading',
    ext_modules=[threadcounter],
    author='Your Name',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Programming Language :: Python :: 3',
        'Programming Language :: C++',
    ],
    python_requires='>=3.6',
)
