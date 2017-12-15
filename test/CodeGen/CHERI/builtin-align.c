// RUN: %cheri_cc1 -o - -O0 -emit-llvm %s
// RUN: %cheri_cc1 -o - -O2 -emit-llvm %s
// RUN: %cheri_cc1 -o - -O0 -emit-llvm %s | FileCheck %s
// RUN: %cheri_cc1 -o - -O2 -emit-llvm %s | FileCheck %s

_Bool is_aligned(void *x) {
  return __builtin_is_aligned(x, 32);
}

_Bool is_aligned2(void *x) {
  return __builtin_is_p2aligned(x, 7);
}

_Bool is_aligned3(void *x, int p2align) {
  return __builtin_is_p2aligned(x, p2align);
}
