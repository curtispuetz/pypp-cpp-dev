# The Py++ development repo for C++ code
This is the C++ development repo for [Py++](https://github.com/curtispuetz/pypp-cli).

This repo contains the C++ implementations that are part of every Py++ project's generated C++ code (e.g. PyList, PySet, PyDict, etc.). It also includes some test code for these implementations.

The code developed in this repo under the 'pypp' directory is ultimately copied to the Py++ source code to be shipped. When Py++ initializes your projects' cpp repo, it copies this directory into there.
