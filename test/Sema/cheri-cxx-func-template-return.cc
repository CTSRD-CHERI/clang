// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// Reduced from <algorithm>
template <class T> bool f();

template<> bool f<char>();
