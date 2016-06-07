// RUN: %clang_cc1 -DCONSTRUCTOR=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// RUN: %clang_cc1 -DFUNC_RET=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics

class type_info {
  public:
    bool operator==(const type_info&) {
      return false;
    }
};

#ifdef CONSTRUCTOR
bool f() {
  type_info t;
  return t == type_info();
}
#endif

#ifdef FUNC_RET
type_info mytypeid() {
  return type_info();
}

bool g() {
  type_info t;
  return t == mytypeid();
}
#endif
