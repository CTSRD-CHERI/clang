// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -ast-dump %s 
// expected-no-diagnostics
template<class C>
C f(C t) {
  return t;
}

template<class C>
class Foo {
  friend C f<>(C);
};

Foo<int> foo;
