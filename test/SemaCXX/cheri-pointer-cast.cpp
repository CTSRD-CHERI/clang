// RUN: %clang_cc1 -triple mips64-unknown-freebsd -target-abi n64 %s -std=c++14 -verify
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi n64 %s -std=c++14 -verify
// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi purecap %s -std=c++14 -verify

typedef unsigned int uint;
typedef unsigned long ulong;

int foo;

#ifdef __CHERI__
#define CAP __capability
#else
#define CAP
#endif

#define DO_CASTS(dest, value) do { \
  dest cstyle = (dest)value; \
  dest functional = dest(value); \
  dest staticCast = static_cast<dest>(value); \
  dest reinterpretCast = reinterpret_cast<dest>(value); \
} while(false)

int main() {
  void* CAP x = (int* CAP)&foo;
  DO_CASTS(ulong, x); //expected-error-re {{static_cast from 'void *{{( __capability)?}}' to 'ulong' (aka 'unsigned long') is not allowed}}
#ifdef __CHERI__
  // expected-warning@-2 3 {{cast from capability type 'void * __capability' to non-capability, non-address type 'ulong' (aka 'unsigned long') is most likely an error}}
#endif
  DO_CASTS(uint, x);
#ifdef __CHERI__
  // expected-error@-2 3 {{cast from capability to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-3 {{static_cast from 'void * __capability' to 'uint' (aka 'unsigned int') is not allowed}}
#else
  // expected-error@-5 3 {{cast from pointer to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-6 {{static_cast from 'void *' to 'uint' (aka 'unsigned int') is not allowed}}
#endif

  void* nocap = &foo;
  DO_CASTS(ulong, nocap); //expected-error-re {{static_cast from 'void *{{( __capability)?}}' to 'ulong' (aka 'unsigned long') is not allowed}}
#ifdef __CHERI_PURE_CAPABILITY__
  // expected-warning@-2 3 {{cast from capability type 'void * __capability' to non-capability, non-address type 'ulong' (aka 'unsigned long') is most likely an error}}
#endif

  DO_CASTS(uint, nocap);
#ifdef __CHERI_PURE_CAPABILITY__
  // expected-error@-2 3 {{cast from capability to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-3 {{static_cast from 'void * __capability' to 'uint' (aka 'unsigned int') is not allowed}}
#else
  // expected-error@-5 3 {{cast from pointer to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-6 {{static_cast from 'void *' to 'uint' (aka 'unsigned int') is not allowed}}
#endif

  // also try casting from nullptr_t
  DO_CASTS(ulong, nullptr); //expected-error {{static_cast from 'nullptr_t' to 'ulong' (aka 'unsigned long') is not allowed}}
  DO_CASTS(uint, nullptr);
#ifdef __CHERI_PURE_CAPABILITY__
  // expected-error@-2 3 {{cast from capability to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-3 {{static_cast from 'nullptr_t' to 'uint' (aka 'unsigned int') is not allowed}}
#else
  // expected-error@-5 3 {{cast from pointer to smaller type 'uint' (aka 'unsigned int') loses information}}
  //expected-error@-6 {{static_cast from 'nullptr_t' to 'uint' (aka 'unsigned int') is not allowed}}
#endif
  return 0;
}
