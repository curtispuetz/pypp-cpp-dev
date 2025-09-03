# ComPy (Compiled Python) C++ development repo
This is the C++ development repo for [ComPy](https://github.com/curtispuetz/compy-cli).

This repo contains the C++ implementations that are part of every ComPy project's generated C++ code (e.g. PyList, PySet, PyDict, etc.). It also includes some test code for these implementations.

The code developed in this repo under the 'compy' directory is ultimately copied to the ComPy source code to be shipped. When ComPy initializes your projects' cpp repo, it copies this directory into there.
