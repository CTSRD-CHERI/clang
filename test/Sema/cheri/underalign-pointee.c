// RUN: %cheri_cc1 -fsyntax-only -verify %s -target-abi purecap

__attribute__((aligned(2)))
typedef
void void2;
void2 *data;

void **invalidCast(void)
{
	void **p = data; // expected-error-re{{cast from 'void2 * __capability' (aka 'void * __capability') to 'void * __capability * __capability' increases required alignment from 2 to {{8|16|32}}}}
	return p;
}
