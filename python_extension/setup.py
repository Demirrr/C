from setuptools import setup, Extension

# Define the extension module
countermodule = Extension(
    'countermodule',
    sources=['countermodule.c'],
    extra_compile_args=['-std=c11'],
)

setup(
    name='countermodule',
    version='1.0',
    description='Python C Extension - Counter class example',
    ext_modules=[countermodule],
    author='Your Name',
    author_email='your.email@example.com',
    url='https://github.com/yourusername/countermodule',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
    ],
    python_requires='>=3.6',
)
