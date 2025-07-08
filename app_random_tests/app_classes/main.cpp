#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

class ClassA {
  public:
    int self_a;
    ClassA(int a) : self_a(a) {
        std::cout << "Constructor Class A" << std::endl;
    }

    int get_a() { return self_a; }
};

class ClassB {
  public:
    ClassA self_class_a;
    ClassB(ClassA class_a) : self_class_a(class_a) {
        std::cout << "Constructor Class B" << std::endl;
    }
    int add(int b) { return self_class_a.get_a() + b; }
};

struct BaseStruct {
    int base_value;
    BaseStruct(int value) : base_value(value) {
        std::cout << "Constructor Base Struct" << std::endl;
    }
    int add(int b) { return base_value + b; }
    int subtract(int b) { return base_value - b; }
};

struct ChildStruct : public BaseStruct {
    int child_value;
    ChildStruct(int value, int b) : child_value(b), BaseStruct(value) {
        std::cout << "Constructor Child Struct" << std::endl;
    }
    int multiply(int b) { return base_value * child_value * b; }
    int add(int b) {
        std::cout << "ChildStruct add called" << std::endl;
        return subtract(b); // implementation is wrong on purpose
    }
};

int main() {
    ClassA _class_a(1);
    ClassB _class_b(_class_a);
    std::cout << _class_b.add(2) << std::endl;
    // inheritance tests
    ChildStruct _child_struct(10, 2);
    std::cout << _child_struct.add(2) << std::endl;
    std::cout << _child_struct.multiply(2) << std::endl;
    std::cout << _child_struct.subtract(2) << std::endl;

    try {
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
