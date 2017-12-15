// RUN: %cheri_cc1 -o - -O0 -emit-llvm -mllvm -debug -mllvm -print-before-all %s
// RsUN: %cheri_cc1 -o - -O2 -emit-llvm %s
// RUN: %cheri_cc1 -o - -O0 -emit-llvm %s | FileCheck %s
// RUN: %cheri_cc1 -o - -O2 -emit-llvm %s | FileCheck %s -check-prefix OPT

_Bool is_aligned(void *x) {
  // CHECK-LABEL: is_aligned(
  return __builtin_is_aligned(x, 32);
}

_Bool is_aligned2(void *x) {
  // CHECK-LABEL: is_aligned2(
  return __builtin_is_p2aligned(x, 7);
}

_Bool is_aligned3(void *x, int p2align) {
  // CHECK-LABEL: is_aligned3(
  return __builtin_is_p2aligned(x, p2align);
}

// Check that constants get folded
_Bool opt_is_aligned1(void *x) {
  // OPT-LABEL: opt_is_aligned1(
  // OPT: ret i1 true
  return __builtin_is_p2aligned(x, 0) && __builtin_is_aligned(x, 1);
}

_Bool opt_is_aligned2() {
  // OPT-LABEL: opt_is_aligned2(
  // OPT: ret i1 false
  // This expression gets folded even at -O0:
  // CHECK-LABEL: opt_is_aligned2(
  // CHECK: ret i1 false
  return __builtin_is_p2aligned(7, 3);
}

_Bool opt_is_aligned3() {
  // OPT-LABEL: opt_is_aligned3(
  // OPT: ret i1 true
  // This expression gets folded even at -O0:
  // CHECK-LABEL: opt_is_aligned3(
  // CHECK: ret i1 true
  return __builtin_is_aligned(1024, 512);
}
