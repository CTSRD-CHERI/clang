// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -std=c++11 -fsyntax-only -verify %s
// expected-no-diagnostics

// adapted from <chrono>, <ratio> and __is_ratio

template <int N>
class A { };

template <bool __v>
struct bool_constant {
  static const bool value = __v;
};

template <class _Tp>
struct __is_A : bool_constant<false> {};

template <int _Num>
struct __is_A<A<_Num> > : bool_constant<true> {};

static_assert(__is_A<A<1> >::value, "Error");
