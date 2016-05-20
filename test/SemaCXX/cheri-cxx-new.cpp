// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -DNEW_DELETE_DECL=1 -fsyntax-only -verify -Wno-implicit-exception-spec-mismatch %s
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -DNEW_ASSIGN=1 -fsyntax-only -verify %s
// expected-no-diagnostics

#ifdef NEW_DELETE_DECL
void* operator new(unsigned long);
void operator delete(void*);
#endif

#ifdef NEW_ASSIGN
class A { };
int main(void) {
  A* a = new A;
  return 0;
}
#endif
