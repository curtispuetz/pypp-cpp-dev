# The Py++ development repo for C++ code
This is the C++ development repo for [Py++](https://github.com/curtispuetz/pypp-cli).

The code under the `pypp` directory is what I call the 'Py++ cpp template'. It is the code that is included in the generated C++ code for every Py++ project, because the C++ code that the Py++ transpiler generates relies on this code.

The project also includes some random test code and some benchmarking using the google benchamrks library.

## Building the project

```text
$ cmake -S . -B build -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBENCHMARK_DOWNLOAD_DEPENDENCIES=true -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
```
