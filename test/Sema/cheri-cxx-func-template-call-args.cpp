// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// reduced from wbuffer_convert::underflow() in <locale>
template <class T>
void min(const T&, const T&);

void underflow() {
  min(0, static_cast<int>(0));
}
