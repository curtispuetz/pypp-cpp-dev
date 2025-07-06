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

int main() {
    ClassA _class_a(1);
    ClassB _class_b(_class_a);
    std::cout << _class_b.add(2) << std::endl;
    try {
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
