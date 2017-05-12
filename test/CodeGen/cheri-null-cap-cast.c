// RUN: %clang_cc1 -triple cheri-unknown-freebsd -emit-llvm -o - %s | FileCheck %s

int main(void) {
  char * __capability p;
  // CHECK: [[P:%.+]] = alloca i8 addrspace(200)*,
  // CHECK: [[VAR0:%.+]] = load i8 addrspace(200)*, i8 addrspace(200)** [[P]]
  // CHECK-NEXT: [[CMP:%.+]] = icmp eq i8 addrspace(200)* [[VAR0]], null
  if (p == (void*)0) {
  }
  // CHECK: [[VAR1:%.+]] = load i8 addrspace(200)*, i8 addrspace(200)** [[P]]
  // CHECK-NEXT: [[CMP1:%.+]] = icmp eq i8 addrspace(200)* null, [[VAR1]]
  else if ((void*)0 == p) {
  }
  return 0;
}
