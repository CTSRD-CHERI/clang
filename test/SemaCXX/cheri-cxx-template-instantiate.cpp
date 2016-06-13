// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -std=c++11 -fsyntax-only -verify %s
// expected-no-diagnostics

// reduced from __find_max_align in <type_traits> 
struct foo { }; 

template <class T> struct find;
template <class H, class T> struct list;

template <class H>
struct find<list<H, foo> > {
  static const long value = 3;
};

template <long V = find<list<foo, foo> >::value>
struct storage { };

class bar {
  storage<0> buf;
};
