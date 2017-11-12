// RUN: %clang_cc1 -triple cheri-unknown-freebsd -o - %s -S -emit-llvm -no-cheri-linker | %cheri_FileCheck %s -check-prefix=NOLINKER
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -o - %s -S -emit-llvm | %cheri_FileCheck %s
static int foo;
int * __capability bar1 = (int* __capability)&foo;
// CHECK: @bar1 = global i32 addrspace(200)* addrspacecast (i32* @foo to i32 addrspace(200)*), align [[$CAP_SIZE]]
// NOLINKER: store i32 addrspace(200)* addrspacecast (i32* @foo to i32 addrspace(200)*), i32 addrspace(200)** @bar1, align [[$CAP_SIZE]]

