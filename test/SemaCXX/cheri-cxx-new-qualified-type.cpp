// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// reduced from uninitialized_copy(...) in <memory>
template <class _ForwardIterator>
void uninitialized_copy() {
  typedef typename _ForwardIterator::value_type value_type;
  new value_type;
}
