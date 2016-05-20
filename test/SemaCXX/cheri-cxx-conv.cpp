// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics

class A {
  public:
    operator int() const { return 42; }
};

int foo(A a) {
  return a;
}
