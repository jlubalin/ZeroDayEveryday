from setuptools import setup, Extension

# 1. Define the extension module
# The first argument is the name of the module you will import in Python.
# The 'sources' list contains the C files that need to be compiled.
steg_module = Extension('stegjpeg', sources=['stegjpeg.c'])

# 2. Run the setup configuration
setup(
    name='stegjpeg',
    version='1.0',
    description='C-extension for image steganography',
    ext_modules=[steg_module]
)