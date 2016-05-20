// RUN: %clang_cc1 -DCOPY_ASSIGN=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify -Wno-implicit-exception-spec-mismatch %s
// RUN: %clang_cc1 -DCOPY_CONVERSION=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify -Wno-implicit-exception-spec-mismatch %s
// RUN: %clang_cc1 -DCOPY_RETURN_ONE_LEVEL=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// RUN: %clang_cc1 -DCOPY_RETURN_TWO_LEVEL=1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics

class A { };

#ifdef COPY_ASSIGN
int main(void) {
  A a1;
  A a2 = a1;
  return 0;
}
#endif

#ifdef COPY_CONVERSION
class B {
  public:
    B(const A& a) { }
};

int main(void) {
  A a;
  B b = B(a);
  return 0;
}
#endif

#ifdef COPY_RETURN_ONE_LEVEL
class B {
  public:
    A getA() {
      return a;
    }

  private:
    A a;
};

int main(void) {
  B b;
  A a = b.getA();
  return 0;
}
#endif

#ifdef COPY_RETURN_TWO_LEVEL
class B {
  public:
    A getA() {
      return reallyGetA();
    }

    A reallyGetA() {
      return a;
    }

  private:
    A a;
};

int main(void) {
  B b;
  A a = b.getA();
  return 0;
}
#endif
