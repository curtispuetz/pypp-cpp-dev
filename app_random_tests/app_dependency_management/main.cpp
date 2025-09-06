#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

#include "py_list.h"
#include "pypp_util/print.h"
#include <functional>
#include <variant>

// NOTE: At the moment, I really like this. Because it basically makes it like
// Python minus:
// - You have to keep the references that you pass alive.
// - But you also have the option to move if you want.

// Decision: Don't use this for functions and methods. I am worried that it
// makes it too complicated and that it could hinder the performance of tight
// loops. For function arguments, you get pass-by-reference by default, or
// pass-by-value when you define the function you wrap the type with Valu[]. If
// you decide pass-by-value, then at the call sites you must either wrap the
// argument with cpy() or mov(). Lastly, the default is a const reference, so if
// you want it non-const, you wrap the type with Mut<>

// A generic wrapper that can either own or reference a dependency
template <typename T> class ZDep {
    std::variant<std::reference_wrapper<T>, T> data_;

  public:
    // Construct with reference
    ZDep(T &ref) : data_(std::ref(ref)) {}

    // Construct by moving
    ZDep(T &&obj) : data_(std::move(obj)) {}

    // Accessors
    T &g() {
        if (std::holds_alternative<std::reference_wrapper<T>>(data_)) {
            return std::get<std::reference_wrapper<T>>(data_).get();
        } else {
            return std::get<T>(data_);
        }
    }
    const T &g() const {
        if (std::holds_alternative<std::reference_wrapper<T>>(data_)) {
            return std::get<std::reference_wrapper<T>>(data_).get();
        } else {
            return std::get<T>(data_);
        }
    }
};

void second_function_with_dependency(ZDep<pypp::PyList<int>> dep) {
    dep.g().append(100);
}

void function_with_dependency(ZDep<pypp::PyList<int>> dep) {
    dep.g().append(42);
    second_function_with_dependency(dep.g());
    pypp::print("Inside function_with_dependency, list contents:", dep.g());
    second_function_with_dependency(std::move(dep)); // Move the dependency
    pypp::print("Inside function_with_dependency after move, list contents:",
                dep.g());
}

class ClassA {
    ZDep<pypp::PyList<int>> zself_dep;
    pypp::PyList<int> &self_dep;

  public:
    ClassA(ZDep<pypp::PyList<int>> dep)
        : zself_dep(std::move(dep)), self_dep(zself_dep.g()) {}
    void m() { pypp::print("Inside ClassA::m, list contents:", self_dep); }
};

class ClassB {
    ZDep<pypp::PyList<int>> self_dep;

  public:
    ClassB(ZDep<pypp::PyList<int>> dep) : self_dep(std::move(dep)) {}
    void m() { pypp::print("Inside ClassB::m, list contents:", self_dep.g()); }
};

// NOTE: Don't support returning a reference. Instead, the reference should be
// injected directly to where it is needed.
//  ZDep<pypp::PyList<int>> returning_dependency(pypp::PyList<int> &list) {}

int main() {
    try {
        // This demonstrates passing-by-reference with a function.
        pypp::PyList<int> my_list = pypp::PyList<int>({1, 2, 3});
        function_with_dependency(my_list); // Pass by reference
        pypp::print("List after function call:", my_list);
        // This demonstrates moving with a function.
        pypp::PyList<int> my_list2 = pypp::PyList<int>({4, 5, 6});
        function_with_dependency(std::move(my_list2)); // Pass by move
        pypp::print("List after move function call:", my_list2);

        // This demonstrates passing-by-reference with a class.
        pypp::PyList<int> my_list3 = pypp::PyList<int>({7, 8, 9});
        ClassA class_a(my_list3); // pass by reference
        my_list3.append(10);
        class_a.m();

        // This demonstrates that you should not move an object if you have
        // references to that object in different places.
        pypp::PyList<int> my_list4 = pypp::PyList<int>({7, 8, 9});
        ClassA class_a2(my_list4); // pass by reference
        my_list4.append(10);
        ClassB class_b(std::move(my_list4)); // pass by move
        class_a2.m(); // Reference to the list in class_a2 is broken.

        return 0;
    } catch (...) {

        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
