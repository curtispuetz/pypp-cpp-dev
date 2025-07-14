#include "py_list.h"

struct MyConfig {
    PyList<int> a = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int b = 1;
    int c = 2;
};

MyConfig MyConfigInstance; // global or static instance