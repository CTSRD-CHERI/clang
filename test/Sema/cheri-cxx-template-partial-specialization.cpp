// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
template <bool __v>
struct bool_constant {
  static const bool value = __v;
};

template <class _Tp>
struct my_is_pointer       : public bool_constant<false> {};

template <class _Tp>
struct my_is_pointer<_Tp*> : public bool_constant<true> {};

template<bool b, class T>
struct my_enable_if { };

template <class T>
struct my_enable_if<true, T> {
  typedef T type;
};

template<class T>
void f(T, typename my_enable_if
                   <
                     my_is_pointer<T>::value, void
                   >::type* = 0) { }

int main(void) {
  int i;
  f(&i);
  return 0;
}
