Tutorial
#########

In C++, constexpr is not part of type. ICE (inlaid constant expression) is a header-only C++11 library making the constexpr be a part of type. It could be useful for writing library code. Sometimes we want to have function overloading based on the arguments being constexpr. There are several use cases why this feature would be useful.

        The fact that an argument is not a constant expression means that we can't use it as a non-type template parameter, 
        as an array bound, inside a static_assert or anything else that requires a constant expression. 
        In addition, this means that the return type of a function can't depend on the value of an argument.

        -- Boost.Hana Documentation

ICE allows us to use function argument as a non-type template parameter, as an array bound, inside a static_assert or anything else that requires a constant expression. In addition, this means that the return type of a function can depend on the value of an argument.

example 1: error handling
===========================

You want to write a smart_assert(bool). Normally if assertion failed, it will give a runtime error. However if argument is constexpr, you want to have a compile-time error.

.. code-block:: cpp

  void smart_assert(bool b) { assert(b); }
  void smart_assert(constexpr bool b) { static_assert(b, "error!"); }

Unfortunately this doesn't work in C++ since constexpr is not part of type. However, you could do this with ICE like this:

.. code-block:: cpp

  void smart_assert(bool b) { assert(b); }
  template<class T> void smart_assert(ice::Const<T> b) { static_assert(b, "error!"); }

  int main() {
      int i = 2;
      smart_assert(ICE(i + i == 5)); // runtime error
      constexpr int j = 2;
      smart_assert(ICE(j + j == 5)); // compile error
  }

Here `ice::Const<T>` keep a expression and contains the information whether the expression is constexpr. `T` is a internal type which you shouldn't care.

Macro ICE is some kind of wrapper for expression. If `expr` is expression, `ICE(expr)` will convert expression to `ice::Const<T>`, otherwise `ice::Nonconst<T>`. However `ice::Nonconst<T>` is convertable to `decltype(expr)`, so if you defined a normal function, it will convert to that one (non-constexpr version).

You could even pass the argument to another function and still keep the constify:

.. code-block:: cpp

    template <class T> double checked_sqrt(ice::Any<T> f) {
        smart_assert(f >= 0);
        return sqrt(f);
    }

    int main() {
        checked_sqrt(ICE(5)); // okay

        double s = -1;
        checked_sqrt(ICE(s)); // runtime error

        s = 5;
        checked_sqrt(ICE(s)); // okay

        checked_sqrt(ICE(-1)); // compile-time error due to static_assert
        checked_sqrt(5); // compile-time error, forgot ICE(...)
    }

Okay, here `ice::Any<T>` is something which could be converted from `Const<T>` and `Nonconst<T>`, the motivation is to ensure the argument is wrappered by ICE(...).

example 2: constexpr overloading
==================================

Another use case for ICE is that you could have different function implementations for constexpr version and non-constexpr version.

e.g. you are implementing a function to calculate fibonacci number. You want to have two versions: constexpr version and non-constexpr version.

.. code-block:: cpp

    // non-constexpr version
    int fib(int t) {
        std::unordered_map<int, int> a;
        a[1] = a[2] = 1;
        for (int i = 2; i <= t; i++) {
          a[i] = a[i - 1] + a[i - 2];
        }
        return a[t];
    }

    constexpr int fibImpl(int t) {
        return t <= 1 ? t : fibImpl(t - 1) + fibImpl(t - 2);
    }

    // constexpr version
    template <class T> constexpr int fib(ice::Const<T> t) {
        static_assert(t >= 0, "");
        return fibImpl(t);
    }

    int main() {
      int k;
      cin >> k;
      cout << fib(ICE(k));

      auto arg = ICE(6);
      static_assert(fib(arg) == 8, "");
    }

In C++14 we could make the API look a little bit better

.. code-block:: cpp

    // non-constexpr version
    int fib(int t) {
        std::unordered_map<int, int> a;
        ... // same as before
    }

    constexpr int fibImpl(int t) {
        return t <= 1 ? t : fibImpl(t - 1) + fibImpl(t - 2);
    }

    // constexpr version
    template <class T> auto fib(ice::Const<T> t) {
        static_assert(t >= 0, "");
        return ICE(fibImpl(t));
    }

    int main() {
      int k;
      cin >> k;
      cout << fib(ICE(k));
      auto ret = fib(ICE(6));
      static_assert(ret == 8, "");
    }

example 3: simulate non-type template parameter
==================================================

ICE is a superset of non-type template parameter. (e.g. non-type template parameter doesn't accept floating point, ICE doesn't have this restriction). 

.. code-block:: cpp

    template <class T>
    auto make_array(ice::Const<T> t) -> std::array<int, T::get()> {
     return {};
    }
    std::vector<int> make_array(size_t n) { return {}; }

    TEST(ice, Array) {
     auto a = make_array(ICE(100));
     static_assert(std::is_same<decltype(a), std::array<int, 100>>::value, "");

     size_t n = 100;
     auto b = make_array(ICE(n));
     static_assert(std::is_same<decltype(b), std::vector<int>>::value, "");
    }

License
=======

Code is licensed under Reciprocal Public License 1.5 (RPL-1.5): https://opensource.org/licenses/RPL-1.5
