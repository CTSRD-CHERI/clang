// RUN: %clang_analyze_cc1 -analyzer-checker=core,debug.ExprInspection -analyzer-config unroll-loops=true -analyzer-stats -verify -std=c++11 %s

void clang_analyzer_numTimesReached();

int getNum();
void foo(int &);
// Testing for loops.
int simple_unroll1() {
  int a[9];
  int k = 42;
  for (int i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{9}}
    a[i] = 42;
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int simple_unroll2() {
  int a[9];
  int k = 42;
  int i;
  for (i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{9}}
    a[i] = 42;
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int simple_no_unroll1() {
  int a[9];
  int k = 42;
  for (int i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    a[i] = 42;
    foo(i);
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int simple_no_unroll2() {
  int a[9];
  int k = 42;
  int i;
  for (i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    a[i] = 42;
    i += getNum();
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int simple_no_unroll3() {
  int a[9];
  int k = 42;
  int i;
  for (i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    a[i] = 42;
    (void)&i;
  }
  int b = 22 / (k - 42); // no-warning
  return 0;
}

int simple_no_unroll4() {
  int a[9];
  int k = 42;
  int i;
  for (i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    a[i] = 42;
    int &j = i;
  }
  int b = 22 / (k - 42); // no-warning
  return 0;
}

int simple_no_unroll5() {
  int a[9];
  int k = 42;
  int i;
  for (i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    a[i] = 42;
    int &j{i};
  }
  int b = 22 / (k - 42); // no-warning
  return 0;
}

int nested_outer_unrolled() {
  int a[9];
  int k = 42;
  int j = 0;
  for (int i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{16}}
    for (j = 0; j < getNum(); ++j) {
      clang_analyzer_numTimesReached(); // expected-warning {{15}}
      a[j] = 22;
    }
    a[i] = 42;
  }
  int b = 22 / (k - 42); // no-warning
  return 0;
}

int nested_inner_unrolled() {
  int a[9];
  int k = 42;
  int j = 0;
  for (int i = 0; i < getNum(); i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{4}}
    for (j = 0; j < 8; ++j) {
      clang_analyzer_numTimesReached(); // expected-warning {{32}}
      a[j] = 22;
    }
    a[i] = 42;
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int nested_both_unrolled() {
  int a[9];
  int k = 42;
  int j = 0;
  for (int i = 0; i < 7; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{7}}
    for (j = 0; j < 6; ++j) {
      clang_analyzer_numTimesReached(); // expected-warning {{42}}
      a[j] = 22;
    }
    a[i] = 42;
  }
  int b = 22 / (k - 42); // expected-warning {{Division by zero}}
  return 0;
}

int simple_known_bound_loop() {
  for (int i = 2; i < 12; i++) {
    // This function is inlined in nested_inlined_unroll1()
    clang_analyzer_numTimesReached(); // expected-warning {{90}}
  }
  return 0;
}

int simple_unknown_bound_loop() {
  for (int i = 2; i < getNum(); i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{10}}
  }
  return 0;
}

int nested_inlined_unroll1() {
  int k;
  for (int i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{9}}
    k = simple_known_bound_loop();    // no reevaluation without inlining
  }
  int a = 22 / k; // expected-warning {{Division by zero}}
  return 0;
}

int nested_inlined_no_unroll1() {
  int k;
  for (int i = 0; i < 9; i++) {
    clang_analyzer_numTimesReached(); // expected-warning {{26}}
    k = simple_unknown_bound_loop();  // reevaluation without inlining
  }
  int a = 22 / k; // no-warning
  return 0;
}
