// RUNNOT: %clang_cc1 -triple cheri-unknown-freebsd %s -Wno-error=cheri-capability-misuse -emit-llvm -o - -O2 | FileCheck %s -check-prefixes BOTH,HYBRID -enable-var-scope
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -Wno-error=cheri-capability-misuse -target-abi purecap - %s -emit-llvm -o - -O2 | FileCheck %s -check-prefixes BOTH,PURECAP -enable-var-scope
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -DWITH_CHERI_PTR %s -emit-llvm -o - -O2 | FileCheck %s -check-prefixes BOTH,HYBRID -enable-var-scope
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -DWITH_CHERI_PTR -target-abi purecap %s -emit-llvm -o - -O2 | FileCheck %s -check-prefixes BOTH,PURECAP -enable-var-scope


extern void* global_ptr;
extern void* __capability global_cap;

#ifdef WITH_CHERI_PTR
#define CHERI_PTR __cheri_ptr
#else
#define CHERI_PTR
#endif

void* __capability c_cast_add_cap(void) {
  void* __capability ptr2cap = (CHERI_PTR void* __capability)global_ptr;
  return ptr2cap;

  // HYBRID-LABEL: define i8 addrspace(200)* @c_cast_add_cap()
  // HYBRID: [[GLOBAL:%.+]] = load i8*, i8** @global_ptr, align 8
  // HYBRID-NEXT: [[RETVAL:%.+]] = addrspacecast i8* [[GLOBAL]] to i8 addrspace(200)*
  // HYBRID-NEXT: ret i8 addrspace(200)* [[RETVAL]]

  // PURECAP-LABEL: define i8 addrspace(200)* @c_cast_add_cap()
  // PURECAP: [[RETVAL:%.+]] = load i8 addrspace(200)*, i8 addrspace(200)* addrspace(200)* @global_ptr
  // PURECAP-NEXT: ret i8 addrspace(200)* [[RETVAL]]
}

void* c_cast_remove_cap(void) {
  void* cap2ptr = (CHERI_PTR void*)global_cap;
  return cap2ptr;
  // HYBRID-LABEL: define i8* @c_cast_remove_cap()
  // HYBRID: [[GLOBAL:%.+]] = load i8 addrspace(200)*, i8 addrspace(200)** @global_cap
  // HYBRID-NEXT: [[RETVAL:%.+]] = addrspacecast i8 addrspace(200)* [[GLOBAL]] to i8*
  // HYBRID-NEXT: ret i8* [[RETVAL]]

  // PURECAP-LABEL: define i8 addrspace(200)* @c_cast_remove_cap()
  // PURECAP: [[RETVAL:%.+]] = load i8 addrspace(200)*, i8 addrspace(200)* addrspace(200)* @global_cap
  // PURECAP-NEXT: ret i8 addrspace(200)* [[RETVAL]]
}
