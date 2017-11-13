// RUN: %cheri_purecap_cc1 -emit-llvm -std=c++11 -DINVALID_ATOMIC_CALL -fsyntax-only -verify %s
// RUN: %cheri_purecap_cc1 -emit-llvm -std=c++11 -o - %s | %cheri_FileCheck %s -implicit-check-not i256
// RUN-TODO: %cheri256_cc1 -triple cheri-unknown-freebsd -target-abi purecap -std=c++11 -S -o - %s | %cheri_FileCheck -check-prefix=ASM %s
// RUN-TODO: %cheri128_cc1 -triple cheri-unknown-freebsd -target-abi purecap -std=c++11 -S -o - %s | %cheri_FileCheck -check-prefix=ASM %s
// RUN: %cheri_purecap_cc1 -std=c++11 -ast-dump %s | %cheri_FileCheck -check-prefix=AST %s
// reduced testcase for libcxx exception_fallback.ipp/new_handler_fallback.ipp

// Module ID and source_filename might contain i256 so we explicitly capture this line
// CHECK: ; ModuleID = '{{.+}}/cheri-std-unexpected-atomicrmw.cpp'
// CHECK: source_filename = "{{.+}}"

typedef void (*handler)();
__attribute__((__require_constant_initialization__)) static handler __handler;

#ifdef INVALID_ATOMIC_CALL
handler set_handler_sync(handler func) noexcept {
  return __sync_lock_test_and_set(&__handler, func); // expected-error {{the __sync_* atomic builtins only work with integers and not capability type 'handler' (aka 'void (* __capability)()').}}
  // AST-NOT: DeclRefExpr {{.*}} '__sync_lock_test_and_set_16' '__int128 (volatile __int128 * __capability, __int128, ...) noexcept'
}

handler get_handler_sync() noexcept {
  return __sync_fetch_and_add(&__handler, (handler)0); // expected-error {{the __sync_* atomic builtins only work with integers and not capability type 'handler' (aka 'void (* __capability)()').}}
  // AST-NOT: DeclRefExpr {{.*}} '__sync_fetch_and_add_16' '__int128 (volatile __int128 * __capability, __int128, ...) noexcept'
}
#endif

// CHECK-NOT: i256

handler set_handler_atomic(handler func) noexcept {
  // CHECK: %func.addr = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: %.atomictmp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: %atomic-temp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: store void () addrspace(200)* %func, void () addrspace(200)* addrspace(200)* %func.addr, align [[$CAP_SIZE]]
  // CHECK: [[VAR_0:%.+]] = load void () addrspace(200)*, void () addrspace(200)* addrspace(200)* %func.addr, align [[$CAP_SIZE]]
  // CHECK: store void () addrspace(200)* [[VAR_0]], void () addrspace(200)* addrspace(200)* %.atomictmp, align [[$CAP_SIZE]]
  // CHECK: [[VAR_1:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %.atomictmp to i8 addrspace(200)*
  // CHECK: [[VAR_2:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %atomic-temp to i8 addrspace(200)*
  // CHECK: call void @__atomic_exchange(i64 zeroext [[$CAP_SIZE]], i8 addrspace(200)* bitcast (void () addrspace(200)* addrspace(200)* @_ZL9__handler to i8 addrspace(200)*), i8 addrspace(200)* [[VAR_1]], i8 addrspace(200)* [[VAR_2]], i32 signext 5)
  return __atomic_exchange_n(&__handler, func, __ATOMIC_SEQ_CST);

  // ASM-LABEL: _Z18set_handler_atomicPFvvE:
  // ASM: ld      $3, %got_page(_ZL9__handler)($1)
  // ASM: daddiu  $3, $3, %got_ofst(_ZL9__handler)
  // ASM: cfromptr        $c1, $c0, $3
  // ASM: csetbounds      $c3, $c1, $25
  // ASM: ld      $3, %call16(__atomic_exchange)($1)
  // ASM: cgetpccsetoffset        $c1, $3
  // ASM: daddiu  $5, $zero, 5
  // ASM: move     $4, $25
  // ASM: csc     $c4, $zero, 64($c11)
  // ASM: cincoffset      $c4, $c2, $zero
  // ASM: clc     $c5, $zero, 64($c11)
  // ASM: move     $gp, $1
  // ASM: csc     $c12, $zero, 32($c11)
  // ASM: cincoffset      $c12, $c1, $zero
  // ASM: csd     $2, $zero, 24($c11)
  // ASM: cjalr   $c12, $c17
}

handler get_handler_atomic() noexcept {
  // CHECK: %atomic-temp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: [[VAR_0:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %atomic-temp to i8 addrspace(200)*
  // CHECK: call void @__atomic_load(i64 zeroext [[$CAP_SIZE]], i8 addrspace(200)* bitcast (void () addrspace(200)* addrspace(200)* @_ZL9__handler to i8 addrspace(200)*), i8 addrspace(200)* [[VAR_0]], i32 signext 5)
  // CHECK: [[VAR_1:%.+]] = load void () addrspace(200)*, void () addrspace(200)* addrspace(200)* %atomic-temp, align [[$CAP_SIZE]]
  // CHECK: ret void () addrspace(200)* [[VAR_1]]
  return __atomic_load_n(&__handler, __ATOMIC_SEQ_CST);

  // ASM-LABEL: _Z18get_handler_atomicv:
  // ASM: ld      $2, %got_page(_ZL9__handler)($1)
  // ASM: daddiu  $2, $2, %got_ofst(_ZL9__handler)
  // ASM: cfromptr        $c1, $c0, $2
  // ASM: csetbounds      $c3, $c1, $25
  // ASM: ld      $2, %call16(__atomic_load)($1)
  // ASM: cgetpccsetoffset        $c1, $2
  // ASM: daddiu  $5, $zero, 5
  // ASM: move     $4, $25
  // ASM: cincoffset      $c4, $c12, $zero
  // ASM: move     $gp, $1
  // ASM: csc     $c12, $zero, 0($c11)
  // ASM: cincoffset      $c12, $c1, $zero
  // ASM: cjalr   $c12, $c17

}

__attribute__((__require_constant_initialization__)) static _Atomic(handler) __atomic_handler;

handler set_handler_c11_atomic(handler func) noexcept {
  // CHECK: %func.addr = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: %.atomictmp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: %atomic-temp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: store void () addrspace(200)* %func, void () addrspace(200)* addrspace(200)* %func.addr, align [[$CAP_SIZE]]
  // CHECK: [[VAR_0:%.+]] = load void () addrspace(200)*, void () addrspace(200)* addrspace(200)* %func.addr, align [[$CAP_SIZE]]
  // CHECK: store void () addrspace(200)* [[VAR_0]], void () addrspace(200)* addrspace(200)* %.atomictmp, align [[$CAP_SIZE]]
  // CHECK: [[VAR_1:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %.atomictmp to i8 addrspace(200)*
  // CHECK: [[VAR_2:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %atomic-temp to i8 addrspace(200)*
  // CHECK: call void @__atomic_exchange(i64 zeroext [[$CAP_SIZE]], i8 addrspace(200)* bitcast (void () addrspace(200)* addrspace(200)* @_ZL16__atomic_handler to i8 addrspace(200)*), i8 addrspace(200)* [[VAR_1]], i8 addrspace(200)* [[VAR_2]], i32 signext 5)
  return __c11_atomic_exchange(&__atomic_handler, func, __ATOMIC_SEQ_CST);
  // ASM-LABEL: _Z22set_handler_c11_atomicPFvvE:
  // ASM: ld      $3, %got_page(_ZL16__atomic_handler)($1)
  // ASM: daddiu  $3, $3, %got_ofst(_ZL16__atomic_handler)
  // ASM: cfromptr        $c1, $c0, $3
  // ASM: csetbounds      $c3, $c1, $25
  // ASM: ld      $3, %call16(__atomic_exchange)($1)
  // ASM: cgetpccsetoffset        $c1, $3
  // ASM: daddiu  $5, $zero, 5
  // ASM: move     $4, $25
  // ASM: csc     $c4, $zero, 64($c11)
  // ASM: cincoffset      $c4, $c2, $zero
  // ASM: clc     $c5, $zero, 64($c11)
  // ASM: move     $gp, $1
  // ASM: csc     $c12, $zero, 32($c11)
  // ASM: cincoffset      $c12, $c1, $zero
  // ASM: csd     $2, $zero, 24($c11)
  // ASM: cjalr   $c12, $c17

}

handler get_handler_c11_atomic() noexcept {
  // CHECK: %atomic-temp = alloca void () addrspace(200)*, align [[$CAP_SIZE]]
  // CHECK: [[VAR_0:%.+]] = bitcast void () addrspace(200)* addrspace(200)* %atomic-temp to i8 addrspace(200)*
  // CHECK: call void @__atomic_load(i64 zeroext [[$CAP_SIZE]], i8 addrspace(200)* bitcast (void () addrspace(200)* addrspace(200)* @_ZL16__atomic_handler to i8 addrspace(200)*), i8 addrspace(200)* [[VAR_0]], i32 signext 5)
  // CHECK: [[VAR_1:%.+]] = load void () addrspace(200)*, void () addrspace(200)* addrspace(200)* %atomic-temp, align [[$CAP_SIZE]]
  // CHECK: ret void () addrspace(200)* [[VAR_1]]
  return __c11_atomic_load(&__atomic_handler, __ATOMIC_SEQ_CST);
  // ASM-LABEL: _Z22get_handler_c11_atomicv:
  // ASM: ld      $2, %got_page(_ZL16__atomic_handler)($1)
  // ASM: daddiu  $2, $2, %got_ofst(_ZL16__atomic_handler)
  // ASM: cfromptr        $c1, $c0, $2
  // ASM: csetbounds      $c3, $c1, $25
  // ASM: ld      $2, %call16(__atomic_load)($1)
  // ASM: cgetpccsetoffset        $c1, $2
  // ASM: daddiu  $5, $zero, 5
  // ASM: move     $4, $25
  // ASM: cincoffset      $c4, $c12, $zero
  // ASM: move     $gp, $1
  // ASM: csc     $c12, $zero, 0($c11)
  // ASM: cincoffset      $c12, $c1, $zero
  // ASM: cjalr   $c12, $c17
}
