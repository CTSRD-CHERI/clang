// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -ast-dump %s | FileCheck %s
template<class T>
void f(T* t) { // CHECK: void (T *__capability)
}
