// RUN: %clang_cc1 -triple cheri-unknown-freebsd -fsyntax-only -verify %s -target-abi purecap

// expected-no-diagnostics
void * __capability x =  (void * __capability)0;
void * __capability y =  (void * __capability)-1;

// RUN: %clang_cc1 -triple cheri-unknown-freebsd -fsyntax-only %s -target-abi purecap -emit-llvm -o - | FileCheck %s
// This is in Sema, but let's also test the codegen here:
// CHECK: @x = addrspace(200) global i8 addrspace(200)* null, align [[$CAP_SIZE:16|32]]
// CHECK: @y = addrspace(200) global i8 addrspace(200)* inttoptr (i64 -1 to i8 addrspace(200)*),  align [[$CAP_SIZE]]
