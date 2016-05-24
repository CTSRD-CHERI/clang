// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
template<class T>
class A {
  T f;
};

class B {};

int main(void) {
  A<B> a1;
  A<B> a2 = a1;
  return 0;
}
