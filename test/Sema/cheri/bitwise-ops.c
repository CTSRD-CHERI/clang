// RUN: %cheri_purecap_cc1 -std=c11 -verify -Wpedantic -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-self-assign -Wno-missing-prototypes -Wno-sign-conversion %s
// also check that the same warnings trigger in C++ modes
// RUN: %cheri_purecap_cc1 -xc++ -verify -Wpedantic -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-self-assign -Wno-missing-prototypes -Wno-sign-conversion %s

// See https://github.com/CTSRD-CHERI/clang/issues/189
typedef __uintcap_t uintptr_t;

void check_xor(void *ptr, uintptr_t cap, int i) {

  // TODO: should storing intcap_t in a smaller type such as int without a cast cause a warning?

  uintptr_t int_and_int = i ^ i; // fine
  // i is promoted to __intcap_t here so the warning triggers:
  uintptr_t int_and_cap = i ^ cap;   // expected-warning{{binary expression on capability and non-capability types: 'int' and 'uintptr_t' (aka '__uintcap_t')}}
  uintptr_t cap_and_int = cap ^ i;   // expected-warning{{using xor on a capability type only operates on the offset; consider using vaddr_t if this is used for pointer hashing or explicitly get the offset with __builtin_cheri_offset_get().}}
  uintptr_t cap_and_cap = cap ^ cap; // expected-warning{{using xor on a capability type only operates on the offset}}

  int int_and_int_2 = i ^ i; // fine
  // i is promoted to __intcap_t here so the warning triggers:
  int int_and_cap_2 = i ^ cap;         // expected-warning{{binary expression on capability and non-capability types: 'int' and 'uintptr_t' (aka '__uintcap_t')}}
  uintptr_t cap_and_int_2 = cap ^ i;   // expected-warning{{using xor on a capability type only operates on the offset}}
  uintptr_t cap_and_cap_2 = cap ^ cap; // expected-warning{{using xor on a capability type only operates on the offset}}

  i ^= i;
  // FIXME: shouldn't this really be an invalid operand error?
  i ^= cap;   // expected-warning{{binary expression on capability and non-capability types}}
  cap ^= i;   // expected-warning{{using xor on a capability type only operates on the offset}}
  cap ^= cap; // expected-warning{{using xor on a capability type only operates on the offset}}

  uintptr_t hash = ptr ^ 0x1234567;                                  // expected-error{{invalid operands to binary expression ('void * __capability' and 'int')}}
  uintptr_t hash2 = (__intcap_t)ptr ^ 0x1234567;                     // expected-warning{{using xor on a capability type only operates on the offset}}
  uintptr_t hash3 = cap ^ 0x1234567;                                 // expected-warning{{using xor on a capability type only operates on the offset}}
  uintptr_t nonsense = (__intcap_t)ptr ^ cap;                        // expected-warning{{using xor on a capability type only operates on the offset}}
  uintptr_t is_this_really_zero = (__intcap_t)ptr ^ (__intcap_t)ptr; // expected-warning{{using xor on a capability type only operates on the offset}}
  uintptr_t is_this_really_zero2 = (__intcap_t)ptr ^ cap;            // expected-warning{{using xor on a capability type only operates on the offset}}
}

void check_shift_left(void *ptr, uintptr_t cap, int i) {
  uintptr_t int_and_int = i << i; // fine
  // i is promoted to __intcap_t here so the warning triggers:
  uintptr_t int_and_cap = i << cap;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  uintptr_t cap_and_int = cap << i;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  uintptr_t cap_and_cap = cap << cap; // expected-warning{{using shifts on a capability type only operates on the offset}}
  i <<= i;
  // FIXME: shouldn't this really be an invalid operand error?
  i <<= cap;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  cap <<= i;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  cap <<= cap; // expected-warning{{using shifts on a capability type only operates on the offset}}
}

void check_shift_right(void *ptr, uintptr_t cap, int i) {
  uintptr_t int_and_int = i >> i; // fine
  // i is promoted to __intcap_t here so the warning triggers:
  uintptr_t int_and_cap = i >> cap;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  uintptr_t cap_and_int = cap >> i;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  uintptr_t cap_and_cap = cap >> cap; // expected-warning{{using shifts on a capability type only operates on the offset}}
  i >>= i;
  // FIXME: shouldn't this really be an invalid operand error?
  i >>= cap;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  cap >>= i;   // expected-warning{{using shifts on a capability type only operates on the offset}}
  cap >>= cap; // expected-warning{{using shifts on a capability type only operates on the offset}}
}

void check_and(void *ptr, uintptr_t cap, int i) {
  uintptr_t int_and_int = i & i; // fine
  // i is promoted to __intcap_t here so the warning triggers:
  uintptr_t int_and_cap = i & cap;   // expected-warning{{binary expression on capability and non-capability types: 'int' and 'uintptr_t' (aka '__uintcap_t')}}
  uintptr_t cap_and_int = cap & i;   // expected-warning{{using bitwise and on capability types may give surprising results;}}
  uintptr_t cap_and_cap = cap & cap; // expected-warning{{using bitwise and on capability types may give surprising results;}}
  i &= i;
  // FIXME: shouldn't this really be an invalid operand error?
  i &= cap;   // expected-warning{{binary expression on capability and non-capability types}}
  cap &= i;   // expected-warning{{using bitwise and on capability types may give surprising results;}}
  cap &= cap; // expected-warning{{using bitwise and on capability types may give surprising results;}}
}

// Bitwise or operations just operate on the offset field and this behaviour should always be fine -> no warnings
void check_or(void *ptr, uintptr_t cap, int i) {
  uintptr_t int_and_int = i | i; // fine
  // TODO: bitwise or of integer and capability should probably be an error since it isn't clear if vaddr or offset is wanted
  // however, by the time we do the check i has been promoted so we don't get a warning
  uintptr_t int_and_cap = i | cap; // expected-warning{{binary expression on capability and non-capability types}}
  uintptr_t cap_and_int = cap | i;
  uintptr_t cap_and_cap = cap | cap;
  i |= i;
  // TODO: bitwise or of integer and capability should probably be an error since it isn't clear if vaddr or offset is wanted
  i |= cap; // expected-warning{{binary expression on capability and non-capability types}}
  cap |= i;
  // TODO: this is also not really sensible since it only uses the offset
  cap |= cap;
}

#ifndef __cplusplus
typedef _Bool bool;
#endif

void set_low_pointer_bits(void *ptr, uintptr_t cap) {
  bool aligned = ptr & 7;     // expected-error{{invalid operands to binary expression ('void * __capability' and 'int')}}
  bool aligned_bad = cap & 7; // expected-warning{{using bitwise and on capability types may give surprising results;}}

  // store flag in low pointer bits
  uintptr_t with_flags = cap | 3; // bitwise or works as expected
  if ((with_flags & 3) == 3) {    // expected-warning{{using bitwise and on capability types may give surprising results;}}
    // clear the flags again:
    with_flags &= ~3; // expected-warning{{using bitwise and on capability types may give surprising results;}}
  }

  ptr &= 3; // expected-error{{invalid operands to binary expression ('void * __capability' and 'int')}}
  cap &= 3; // expected-warning{{using bitwise and on capability types may give surprising results;}}

  ptr &= ~3; // expected-error{{invalid operands to binary expression ('void * __capability' and 'int')}}
  cap &= ~3; // expected-warning{{using bitwise and on capability types may give surprising results;}}

  ptr &= cap; // expected-error{{invalid operands to binary expression ('void * __capability' and 'uintptr_t' (aka '__uintcap_t'))}}
  cap &= cap; // expected-warning{{using bitwise and on capability types may give surprising results;}}

  ptr &= ~cap; // expected-error{{invalid operands to binary expression ('void * __capability' and 'uintptr_t' (aka '__uintcap_t'))}}
  cap &= ~cap; // expected-warning{{using bitwise and on capability types may give surprising results;}}
}

// verify that we warn for the QMutexLocker issue:
void do_unlock(void);
void this_broke_qmutex(uintptr_t mtx) {
  if ((mtx & (uintptr_t)1) == (uintptr_t)1) { // expected-warning{{using bitwise and on capability types may give surprising results;}}
    do_unlock();
  }
}

extern void __assert(int);
#define assert(expr) __assert(expr);

// Test that the the macros from cheri.h work as expected
#include <cheri.h>

void check_without_macros(void *mtx) {
  __uintcap_t u = (__uintcap_t)mtx;
  u |= 1;
  // Bug: will always be false
  // TODO: we should also warn about always false here
  if ((u & 1) == 1) { // expected-warning{{using bitwise and on capability types may give surprising results;}}
    do_unlock();
  }
  // clear the bit again, warning should trigger
  u &= ~1; // expected-warning{{using bitwise and on capability types may give surprising results;}}
}

void check_with_macros(void *mtx) {
  // This should not trigger any warnings
  __uintcap_t u = (__uintcap_t)mtx;
  u = cheri_set_low_ptr_bits(u, 1);
  if (cheri_get_low_ptr_bits(u, 3) == 1) {
    do_unlock();
  }
  u = cheri_clear_low_ptr_bits(u, 3);
}

void check_bad_macro_values(void *mtx) {
  __uintcap_t u = (__uintcap_t)mtx;
  // Bad replacement of u &= ~3: should trigger unused result warning
  cheri_clear_low_ptr_bits(u, 3); // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}
  // Bad replacement of u | 3: should trigger unused result warning
  cheri_set_low_ptr_bits(u, 3); // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}
  // get should also have warn unused result
  cheri_get_low_ptr_bits(u, 3); // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}

  // call the implementation directly
  __cheri_clear_low_ptr_bits(u, 3); // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}
  __cheri_set_low_ptr_bits(u, 3);   // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}
  __cheri_get_low_ptr_bits(u, 3);   // expected-warning{{ignoring return value of function declared with 'warn_unused_result' attribute}}

  // getting or clearing too many bits should warn:
  unsigned long value;
  value = cheri_get_low_ptr_bits(u, 32); // expected-error{{static_assert failed "Should only use the low 5 pointer bits"}}
  value = cheri_get_low_ptr_bits(u, 31); // This is fine

  // Check that the other macros also work in if statements
  if (cheri_set_low_ptr_bits(u, 3) == 1) {
    do_unlock();
  }
  if (cheri_clear_low_ptr_bits(u, 3) == 1) {
    do_unlock();
  }
}
