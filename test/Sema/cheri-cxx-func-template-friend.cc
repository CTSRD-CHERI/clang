// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -std=c++11 -fsyntax-only -verify %s
// expected-no-diagnostics
template<class _CharT>
class basic_string;

template<class _CharT>
basic_string<_CharT>
operator+(const _CharT*, const basic_string<_CharT>&);

template<class _CharT>
class basic_string {
  friend basic_string operator+<>(const _CharT*, const basic_string&);
};
extern template class basic_string<char>;
