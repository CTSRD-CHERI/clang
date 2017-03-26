// taken from temporaries.cpp (which crashed when run with target cheri)

// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi purecap -std=c++11 -DCHECK_ERROR -fsyntax-only -verify %s
// RUN: %clang_cc1 -emit-llvm %s -o - -triple cheri-unknown-freebsd -target-abi purecap -std=c++11 | FileCheck %s


namespace PR20227 {
  struct A { ~A(); };
  struct B { virtual ~B(); };
  struct C : B {};

  A &&a = dynamic_cast<A&&>(A{});  // this is valid even with -fno-rtti as it only does lifetime extension
  // CHECK: @_ZN7PR202271aE = addrspace(200) global %"struct.PR20227::A" addrspace(200)* null, align 32
  // CHECK: @_ZGRN7PR202271aE_ = internal addrspace(200) global %"struct.PR20227::A" zeroinitializer, align 1

#ifdef CHECK_ERROR
  // RTTI should be off by default with CHERI
  B &&b = dynamic_cast<C&&>(dynamic_cast<B&&>(C{}));  // expected-error {{cannot use dynamic_cast with -fno-rtti}}
#endif

  B &&c = static_cast<C&&>(static_cast<B&&>(C{}));
  // CHECK: @_ZN7PR202271cE = addrspace(200) global %"struct.PR20227::B" addrspace(200)* null, align 32
  // CHECK: @_ZGRN7PR202271cE_ = internal addrspace(200) global %"struct.PR20227::C" zeroinitializer, align 32

}


// CHECK: define internal void @__cxx_global_var_init() #0 {
// CHECK:   %0 = call i32 @__cxa_atexit(void (i8 addrspace(200)*) addrspace(200)* addrspacecast (void (i8 addrspace(200)*)* bitcast (void (%"struct.PR20227::A" addrspace(200)*)* @_ZN7PR202271AD1Ev to void (i8 addrspace(200)*)*) to void (i8 addrspace(200)*) addrspace(200)*), i8 addrspace(200)* getelementptr inbounds (%"struct.PR20227::A", %"struct.PR20227::A" addrspace(200)* @_ZGRN7PR202271aE_, i32 0, i32 0), i8 addrspace(200)* @__dso_handle) #2
// CHECK:   store %"struct.PR20227::A" addrspace(200)* @_ZGRN7PR202271aE_, %"struct.PR20227::A" addrspace(200)* addrspace(200)* @_ZN7PR202271aE, align 32
// CHECK:   ret void


// CHECK: define internal void @__cxx_global_var_init.1() #0 {
// CHECK:   call void @llvm.memset.p200i8.i64(i8 addrspace(200)* bitcast (%"struct.PR20227::C" addrspace(200)* @_ZGRN7PR202271cE_ to i8 addrspace(200)*), i8 0, i64 32, i32 32, i1 false)
// CHECK:   call void @_ZN7PR202271CC1Ev(%"struct.PR20227::C" addrspace(200)* @_ZGRN7PR202271cE_) #2
// CHECK:   %0 = call i32 @__cxa_atexit(void (i8 addrspace(200)*) addrspace(200)* addrspacecast (void (i8 addrspace(200)*)* bitcast (void (%"struct.PR20227::C" addrspace(200)*)* @_ZN7PR202271CD1Ev to void (i8 addrspace(200)*)*) to void (i8 addrspace(200)*) addrspace(200)*), i8 addrspace(200)* bitcast (%"struct.PR20227::C" addrspace(200)* @_ZGRN7PR202271cE_ to i8 addrspace(200)*), i8 addrspace(200)* @__dso_handle) #2
// CHECK:   store %"struct.PR20227::B" addrspace(200)* getelementptr inbounds (%"struct.PR20227::C", %"struct.PR20227::C" addrspace(200)* @_ZGRN7PR202271cE_, i32 0, i32 0), %"struct.PR20227::B" addrspace(200)* addrspace(200)* @_ZN7PR202271cE, align 32
// CHECK:   ret void
