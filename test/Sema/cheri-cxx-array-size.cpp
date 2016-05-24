// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
template<class T>
void f(T elem) {
  T t[1] = { elem };
}

class A { };

int main() {
  A a;
  f(a);
  return 0;
}
