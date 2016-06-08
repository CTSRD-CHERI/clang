// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// Test case simplified from random_shuffle() in  <algorithm>

class A { };

void f(const A&);

template <class T>
void g() {
  T t;
  f(A(t));
}
