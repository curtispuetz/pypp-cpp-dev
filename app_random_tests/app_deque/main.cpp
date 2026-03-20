#include "py_deque.h"
#include "py_enumerate.h"
#include "pypp_util/print.h"
#include <cstdlib>
#include <gtest/gtest.h>
#include <pypp_util/main_error_handler.h>

// ── Construction ─────────────────────────────────────────────────────────────

TEST(DequeTest, EmptyConstruction) {
    pypp::PyDeque<int> d(pypp::PyList<int>{});
    ASSERT_EQ(d.len(), 0);
    ASSERT_FALSE(d.maxlen().has_value());
}

TEST(DequeTest, EmptyConstruction2) {
    pypp::PyDeque<int> d;
    ASSERT_EQ(d.len(), 0);
    ASSERT_FALSE(d.maxlen().has_value());
}

TEST(DequeTest, InitializerListConstruct) {
    pypp::PyDeque<int> d(pypp::PyList{1, 2, 3});
    ASSERT_EQ(d.len(), 3);
    ASSERT_EQ(d[0], 1);
    ASSERT_EQ(d[1], 2);
    ASSERT_EQ(d[2], 3);
}

TEST(DequeTest, ConstructWithMaxlen) {
    pypp::PyDeque<int> d(pypp::PyList{1, 2, 3, 4, 5}, 3);
    // Only the last 3 elements should be kept
    ASSERT_EQ(d.len(), 3);
    ASSERT_TRUE(d.maxlen().has_value());
    ASSERT_EQ(d.maxlen().value(), 3);
    ASSERT_EQ(d[0], 3);
    ASSERT_EQ(d[1], 4);
    ASSERT_EQ(d[2], 5);
}

TEST(DequeTest, FromSet) {
    pypp::PySet<int> s{1, 2, 3};
    pypp::PyDeque<int> d(s);
    // The order of elements from the set is not guaranteed, but all elements
    // should be present
    ASSERT_EQ(d.len(), 3);
}

TEST(DequeTest, FromDict) {
    pypp::PyDict<int, int> dict{{1, 10}, {2, 20}, {3, 30}};
    pypp::PyDeque<int> d(dict);
    // The order of keys from the dict is not guaranteed, but all keys should
    // be present
    ASSERT_EQ(d.len(), 3);
}

// ── append / appendleft
// ───────────────────────────────────────────────────────

TEST(DequeTest, Append) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2});
    d.append(3);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3}), d);
}

TEST(DequeTest, AppendLeft) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2});
    d.appendleft(0);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{0, 1, 2}), d);
}

TEST(DequeTest, AppendEnforcesMaxlenRight) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3}, 3);
    d.append(4);
    // 1 should have been dropped from the left
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{2, 3, 4}), d);
}

TEST(DequeTest, AppendLeftEnforcesMaxlenLeft) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3}, 3);
    d.appendleft(0);
    // 3 should have been dropped from the right
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{0, 1, 2}), d);
}

// ── pop / popleft
// ─────────────────────────────────────────────────────────────

TEST(DequeTest, Pop) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    int val = d.pop();
    ASSERT_EQ(val, 3);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2}), d);
}

TEST(DequeTest, PopLeft) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    int val = d.popleft();
    ASSERT_EQ(val, 1);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{2, 3}), d);
}

TEST(DequeTest, PopFromEmptyThrows) {
    pypp::PyDeque<int> d(pypp::PyList<int>{});
    ASSERT_THROW(d.pop(), pypp::IndexError);
}

TEST(DequeTest, PopLeftFromEmptyThrows) {
    pypp::PyDeque<int> d(pypp::PyList<int>{});
    ASSERT_THROW(d.popleft(), pypp::IndexError);
}

// ── extend / extendleft
// ───────────────────────────────────────────────────────

TEST(DequeTest, Extend) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2});
    auto other = pypp::PyList<int>{3, 4};
    d.extend(other);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3, 4}), d);
}

TEST(DequeTest, ExtendLeft) {
    pypp::PyDeque<int> d(pypp::PyList<int>{3, 4});
    auto other = pypp::PyList<int>{1, 2};
    d.extendleft(other);
    // extendleft appends each element to the front one at a time,
    // so 1 is prepended first, then 2 → final order: 2, 1, 3, 4
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{2, 1, 3, 4}), d);
}

TEST(DequeTest, ExtendWithMaxlen) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2}, 3);
    auto other = pypp::PyList<int>{3, 4, 5};
    d.extend(other);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{3, 4, 5}), d);
}

// ── insert
// ────────────────────────────────────────────────────────────────────

TEST(DequeTest, Insert) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 3, 4});
    d.insert(1, 2);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3, 4}), d);
}

TEST(DequeTest, InsertNegativeIndex) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 4});
    d.insert(-1, 3); // Insert before last element
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3, 4}), d);
}

TEST(DequeTest, InsertAtMaxlenThrows) {
    pypp::PyDeque<int> d(pypp::PyList({1, 2, 3}), 3);
    ASSERT_THROW(d.insert(1, 99), pypp::IndexError);
}

// ── remove
// ────────────────────────────────────────────────────────────────────

TEST(DequeTest, Remove) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3, 2});
    d.remove(2);
    // Only the first occurrence should be removed
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 3, 2}), d);
}

TEST(DequeTest, RemoveNotFoundThrows) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    ASSERT_THROW(d.remove(99), pypp::ValueError);
}

// ── rotate
// ────────────────────────────────────────────────────────────────────

TEST(DequeTest, RotateRight) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3, 4, 5});
    d.rotate(2);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{4, 5, 1, 2, 3}), d);
}

TEST(DequeTest, RotateLeft) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3, 4, 5});
    d.rotate(-2);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{3, 4, 5, 1, 2}), d);
}

TEST(DequeTest, RotateByFullSizeIsIdentity) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    d.rotate(3);
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3}), d);
}

TEST(DequeTest, RotateEmptyDequeNoOp) {
    pypp::PyDeque<int> d(pypp::PyList<int>{});
    d.rotate(5); // should not crash
    ASSERT_EQ(d.len(), 0);
}

TEST(DequeTest, RotateDefault1) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    d.rotate(); // equivalent to rotate(1)
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{3, 1, 2}), d);
}

// ── reverse
// ───────────────────────────────────────────────────────────────────

TEST(DequeTest, Reverse) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    d.reverse();
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{3, 2, 1}), d);
}

// ── copy
// ──────────────────────────────────────────────────────────────────────

TEST(DequeTest, Copy) {
    pypp::PyDeque<int> d(pypp::PyList({1, 2, 3}), 5);
    pypp::PyDeque<int> c = d.copy();
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3}), c);
    ASSERT_EQ(c.maxlen().value(), 5);
    c.append(99);
    // Original is unaffected
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3}), d);
}

// ── clear
// ─────────────────────────────────────────────────────────────────────

TEST(DequeTest, Clear) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    d.clear();
    ASSERT_EQ(d.len(), 0);
}

// ── count
// ─────────────────────────────────────────────────────────────────────

TEST(DequeTest, Count) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 2, 3, 2});
    ASSERT_EQ(d.count(2), 3);
    ASSERT_EQ(d.count(5), 0);
}

// ── index
// ─────────────────────────────────────────────────────────────────────

TEST(DequeTest, Index) {
    pypp::PyDeque<int> d(pypp::PyList<int>{10, 20, 30});
    ASSERT_EQ(d.index(20), 1);
}

TEST(DequeTest, IndexWithRange) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3, 2, 1});
    ASSERT_EQ(d.index(2, 2, 5), 3);
}

TEST(DequeTest, IndexWithRange2) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3, 2, 1});
    ASSERT_EQ(d.index(2, 2), 3);
}

TEST(DequeTest, IndexNotFoundThrows) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    ASSERT_THROW(d.index(99), pypp::ValueError);
}

// ── contains
// ──────────────────────────────────────────────────────────────────

TEST(DequeTest, Contains) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    ASSERT_TRUE(d.contains(2));
    ASSERT_FALSE(d.contains(99));
}

// ── access with [] operator

TEST(DequeTest, BracketOperator) {
    pypp::PyDeque<int> d(pypp::PyList<int>{10, 20, 30});
    ASSERT_EQ(d[0], 10);
    ASSERT_EQ(d[1], 20);
    ASSERT_EQ(d[2], 30);
}

// ── dg (bounds-checked access)
// ────────────────────────────────────────────────

TEST(DequeTest, DgPositiveIndex) {
    pypp::PyDeque<int> d(pypp::PyList<int>{10, 20, 30});
    ASSERT_EQ(d.dg(0), 10);
    ASSERT_EQ(d.dg(2), 30);
}

TEST(DequeTest, DgNegativeIndex) {
    pypp::PyDeque<int> d(pypp::PyList<int>{10, 20, 30});
    ASSERT_EQ(d.dg(-1), 30);
    ASSERT_EQ(d.dg(-3), 10);
}

TEST(DequeTest, DgOutOfRangeThrows) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    ASSERT_THROW(d.dg(5), pypp::IndexError);
    ASSERT_THROW(d.dg(-4), pypp::IndexError);
}

// ── comparison operators
// ──────────────────────────────────────────────────────

TEST(DequeTest, EqualityOperator) {
    pypp::PyDeque<int> a(pypp::PyList<int>{1, 2, 3});
    pypp::PyDeque<int> b(pypp::PyList<int>{1, 2, 3});
    pypp::PyDeque<int> c(pypp::PyList<int>{1, 2, 4});
    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
    ASSERT_TRUE(a != c);
}

TEST(DequeTest, OtherComparisonOperators) {
    pypp::PyDeque<int> a(pypp::PyList<int>{1, 2, 3});
    pypp::PyDeque<int> b(pypp::PyList<int>{1, 2, 4});
    ASSERT_TRUE(a < b);
    ASSERT_FALSE(b < a);
    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(b > a);
    ASSERT_TRUE(b >= a);
}

// ── operator+ and operator+= ─────────────────────────────────────────────────

TEST(DequeTest, Concatenation) {
    pypp::PyDeque<int> a(pypp::PyList<int>{1, 2});
    pypp::PyDeque<int> b(pypp::PyList<int>{3, 4});
    pypp::PyDeque<int> c = a + b;
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3, 4}), c);
}

TEST(DequeTest, ConcatenationAssignment) {
    pypp::PyDeque<int> a(pypp::PyList<int>{1, 2});
    pypp::PyDeque<int> b(pypp::PyList<int>{3, 4});
    a += b;
    ASSERT_EQ(pypp::PyDeque(pypp::PyList{1, 2, 3, 4}), a);
}

// ── iterator support
// ──────────────────────────────────────────────────────────

TEST(DequeTest, RangeForLoop) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    int sum = 0;
    for (int v : d) {
        sum += v;
    }
    ASSERT_EQ(sum, 6);
}

TEST(DequeTest, Enumerate) {
    pypp::PyDeque<int> d(pypp::PyList<int>{10, 20, 30});
    for (const auto &[i, val] : pypp::PyEnumerate(d)) {
        ASSERT_EQ(val, (i + 1) * 10);
    }
}

// ── print / operator<<
// ────────────────────────────────────────────────────────

TEST(DequeTest, PrintNoMaxlen) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3});
    std::ostringstream oss;
    oss << d;
    ASSERT_EQ(oss.str(), "deque([1, 2, 3])");
}

TEST(DequeTest, PrintWithMaxlen) {
    pypp::PyDeque<int> d(pypp::PyList<int>{1, 2, 3}, 5);
    std::ostringstream oss;
    oss << d;
    ASSERT_EQ(oss.str(), "deque([1, 2, 3], maxlen=5)");
}

TEST(DequeTest, PrintMethod) {
    pypp::PyDeque<int> d;
    std::ostringstream oss;
    oss << d;
    ASSERT_EQ(oss.str(), "deque([])");
}
