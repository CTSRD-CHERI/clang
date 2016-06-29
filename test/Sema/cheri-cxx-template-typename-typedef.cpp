// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -std=c++11 -fsyntax-only -verify %s
// expected-no-diagnostics
// reduced test case from <string>

template<bool b>
struct bool_constant {
  static const bool value = b;
};

template<class T1, class T2>
class is_same : public bool_constant<false> { };

template<class T>
class is_same<T, T> : public bool_constant<true> { };

struct Bar {
  typedef char value_type;
};

template<class T, class U>
class Foo {
  typedef typename T::value_type value_type;
  public:
    static const bool result = is_same<typename T::value_type, U>::value;
};

static_assert(Foo<Bar, char>::result, "Should be true");
