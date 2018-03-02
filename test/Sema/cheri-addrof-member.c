// RUN: %cheri_cc1 -fsyntax-only  %s -verify

// See https://github.com/CTSRD-CHERI/clang/issues/185 and https://github.com/CTSRD-CHERI/clang/issues/180

struct astruct {
  int member1;
  int member2;
  int buf[10];
};


int * __capability
func(struct astruct * __capability asp)
{
  return &(asp->member2); // no-diagnostics
}

int *__capability
func2(int *__capability asp) {
  return &(asp[42]); // no-diagnostics
}

struct as {
  char name[10];
  int value;
  __intcap_t intcap;
  struct astruct structmember;
  struct as *structptr;
  struct as *__capability structcap;
  struct as *structptrarray[10];
  struct as *__capability structcaparray[10];
};

void nested_array(struct as *__capability asc) {
  char *__capability namep = &asc->name[0]; // no-diagnostics
}

void nested_member(struct as *__capability asc) {
  int *__capability i = &asc->structmember.member1; // no-diagnostics
}

void nested_member_subscript(struct as *__capability asc) {
  int *__capability i = &asc->structmember.buf[0]; // no-diagnostics
}

void deref_member_cap(struct as *__capability asc) {
  // If the -> operator is a capability that should be fine too:
  int *__capability i = &asc->structcap->value;                 // no-diagnostics
  int *__capability i2 = &asc->structcap->structmember.member1; // no-diagnostics
}

void nested_arraycap_access(struct as *__capability asc) {
  // And the same for the [] operator
  int *__capability i = &asc->structcaparray[0]->value;     // no-diagnostics
  char *__capability i2 = &asc->structcaparray[0]->name[0]; // no-diagnostics
}

// The following are not okay:

void deref_member_ptr(struct as *__capability asc) {
  // But If the chain includes a pointer -> operator the inferring should stop there
  int *__capability i = &asc->structptr->value;                 // expected-error {{converting non-capability type 'int *' to capability type 'int * __capability' without an explicit cast}}
  int *__capability i2 = &asc->structptr->structmember.member1; // expected-error {{converting non-capability type 'int *' to capability type 'int * __capability' without an explicit cast}}
}

void nested_arrayptr_access(struct as *__capability asc) {
  // And the same for the [] operator
  int *__capability i = &asc->structptrarray[0]->value;     // expected-error {{converting non-capability type 'int *' to capability type 'int * __capability' without an explicit cast}}
  char *__capability i2 = &asc->structptrarray[0]->name[0]; // expected-error {{converting non-capability type 'char *' to capability type 'char * __capability' without an explicit cast}}
}

void addrof_uintcap_t(__uintcap_t cap, struct as *__capability asc, struct as *asp) {
  // Also taking the address of a uintcap_t should not be a capability!
  __uintcap_t *__capability i = &cap;         // expected-error{{converting non-capability type '__uintcap_t *' to capability type '__uintcap_t * __capability'}}
  __intcap_t *__capability i2 = &asc->intcap; // no-diagnostics
  __intcap_t *__capability i3 = &asp->intcap; // expected-error{{converting non-capability type '__intcap_t *' to capability type '__intcap_t * __capability'}}
}


// This started crashing in CodeGen after recursively resolving memberexprs
void a(void) {
  void * __capability b;
  void *__capability *__capability c = &b; // expected-error{{converting non-capability type 'void * __capability *' to capability type 'void * __capability * __capability' without an explicit cast}}
}
