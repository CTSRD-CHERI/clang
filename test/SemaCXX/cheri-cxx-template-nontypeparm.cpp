// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics

// Reduced test case from <bitset>

void min(const int& __a, const int& __b);

template<int _Size>
class Foo {
  public:
    void f(int __pos) {
      min(__pos, _Size);
    }
};

int main(void) {
  return 0;
}
