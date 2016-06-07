// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics

// type_info type for 'typeid' to work.
namespace std { class type_info; }

void f() {
  const std::type_info& t = typeid(bool);
}
