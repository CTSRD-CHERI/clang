// Check that we can set bounds on addrof expressions
// REQUIRES: asserts
// RUN: rm -f %t.dbg
// RUN: %cheri_purecap_cc1 -emit-llvm -cheri-bounds=everywhere-unsafe -xc %s -o /dev/null \
// RUN:   -mllvm -debug-only=cheri-bounds -Rcheri-subobject-bounds -verify 2>%t.dbg
// RUN: FileCheck -input-file %t.dbg %s
// also check when compiled as C++
// RUN: rm -f %t.dbg
// RUN: %cheri_purecap_cc1 -emit-llvm -cheri-bounds=everywhere-unsafe -xc++ %s -o /dev/null \
// RUN:   -mllvm -debug-only=cheri-bounds -Rcheri-subobject-bounds -verify 2>%t.dbg
// RUN: FileCheck -input-file %t.dbg %s

// Check that we can opt out of setting bounds on addrof expressions

struct NoBoundsPls {
    int data;
} __attribute__((cheri_no_subobject_bounds));


struct WithBoundsPls {
    int data;
};

struct HasMemberOfTypeNoBoundsPls {
  int before;
  struct NoBoundsPls nested;
  int after;
};

struct HasFieldWithOptOut {
    int first __attribute__((cheri_no_subobject_bounds));
    int second;
};

// FIXME: what should the opt-out annotation do with multiple levels of member accesses?
// e.g. &foo.a.b.c with b annotated: ignore the annotation and set bounds on c?
// with &foo.a.b.c and c annotated: no bounds at all or set the bounds to the bounds of c?

void do_stuff(void* data);

void test(struct WithBoundsPls* w, struct NoBoundsPls* n, struct HasMemberOfTypeNoBoundsPls* t, struct HasFieldWithOptOut* f) {
  // annotation on the type (applies to all subelements)
  do_stuff(&((struct WithBoundsPls*)n)->data); // cast avoids the bounds setting
  // expected-remark@-1{{setting sub-object bounds for field 'data' (pointer to 'int') to 4 bytes}}
  // CHECK: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'int' addrof to 4
  struct NoBoundsPls n2;
  do_stuff(n); // just passing on, no bounds
  do_stuff(&n[0]); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (array type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: Found array subscript -> opt-out: array type declaration has opt-out attribute -> not setting bounds
  do_stuff(&n[2]); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (array type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: Found array subscript -> opt-out: array type declaration has opt-out attribute -> not setting bounds
  do_stuff((struct WithBoundsPls*)n); // Setting bounds here
  do_stuff(&n2); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (expression declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: opt-out: expression declaration has opt-out attribute -> not setting bounds
  do_stuff(&n->data); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (base type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type declaration has opt-out attribute -> not setting bounds
  do_stuff(&n2.data); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (base type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type declaration has opt-out attribute -> not setting bounds

  // field annotation:
  do_stuff(&f->first); // expected-remark{{not setting bounds for first (field has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field has opt-out attribute -> not setting bounds
  do_stuff(&f->second); // expected-remark{{setting sub-object bounds for field 'second' (pointer to 'int') to 4 bytes}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'int' addrof to 4

  // nested type with annotations
  do_stuff(&t->before); // expected-remark{{setting sub-object bounds for field 'before' (pointer to 'int') to 4 bytes}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'int' addrof to 4
  do_stuff(&t->nested); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (field type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field type declaration has opt-out attribute -> not setting bounds
  do_stuff(&t->nested.data); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (base type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type declaration has opt-out attribute -> not setting bounds
  do_stuff(&t->after); // expected-remark{{setting sub-object bounds for field 'after' (pointer to 'int') to 4 bytes}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'int' addrof to 4

  // Check the opt-out with parenexprs
  do_stuff(&((t)->nested)); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (field type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field type declaration has opt-out attribute -> not setting bounds
  do_stuff(&(((t)->nested).data)); // expected-remark-re{{not setting bounds for '{{(struct )?}}NoBoundsPls' (base type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type declaration has opt-out attribute -> not setting bounds
  do_stuff(&(f->first)); // expected-remark{{not setting bounds for first (field has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field has opt-out attribute -> not setting bounds
}

// opt-out on variable decl (see ncurses source code)
static char* stringbuf;
// TODO:
// static char* stringbuf_opt_out __attribute__((cheri_no_subobject_bounds("array-index")));
static char* stringbuf_opt_out __attribute__((cheri_no_subobject_bounds));

void test_stringbuf(int next_free) {
  do_stuff(&stringbuf[next_free]); // TODO: maybe don't do this by default for char[]?
  // expected-remark@-1{{setting sub-object bounds for pointer to 'char' to 1 bytes}}
  // CHECK-NEXT: subobj bounds check: Found array subscript -> Index is not a constant (probably in a per-element loop) -> Bounds mode is everywhere-unsafe -> setting bounds for 'char' addrof to 1
  do_stuff(&stringbuf_opt_out[next_free]);
  // expected-remark@-1{{not setting bounds for 'char * __capability __attribute__((cheri_no_subobject_bounds))' (array base type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: Found array subscript -> opt-out: array base type has opt-out attribute -> not setting bounds

  // Due to the parenthesized expression this was previous not parsed as an
  // array subscript expression so the bounds opt out annotation was not parsed.
  do_stuff(&((stringbuf_opt_out)[(next_free)]));
  // expected-remark@-1{{not setting bounds for 'char * __capability __attribute__((cheri_no_subobject_bounds))' (array base type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: Found array subscript -> opt-out: array base type has opt-out attribute -> not setting bounds


}

struct a {
  int b;
};

// This previously crashed:
struct a c(void) {
  struct a d[1];
  do_stuff(&d->b); // expected-remark{{setting sub-object bounds for field 'b' (pointer to 'int') to 4 bytes}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'int' addrof to 4
  return d[0];
}

typedef struct WithBoundsPls NoBoundsTypedef __attribute__((cheri_no_subobject_bounds));
typedef __attribute__((cheri_no_subobject_bounds)) struct WithBoundsPls NoBoundsTypedef2;
typedef struct WithBoundsPls __attribute__((cheri_no_subobject_bounds)) NoBoundsTypedef3;
__attribute__((cheri_no_subobject_bounds)) typedef struct WithBoundsPls NoBoundsTypedef4;



// example from libexpat (taking address of struct encoding that is the later casted back to normal_ecoding)
struct encoding;
typedef __attribute__((cheri_no_subobject_bounds)) struct encoding ENCODING;
typedef struct encoding ENCODING2;

struct __attribute__((cheri_no_subobject_bounds)) encoding {
  int i;
};

struct normal_encoding {
  ENCODING enc;
  ENCODING2 enc2;
  unsigned char type[256];
};

struct unknown_encoding {
  struct normal_encoding normal;
  void *userData;
};

ENCODING* test_expat_error(struct unknown_encoding *e) {
  return &(e->normal.enc); // expected-remark-re {{not setting bounds for 'ENCODING' (aka '{{(struct )?}}encoding') (field type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field type has opt-out attribute -> not setting bounds
}

ENCODING2* test_expat_error2(struct unknown_encoding *e) {
  return &(e->normal.enc2); // expected-remark-re {{not setting bounds for '{{(struct )?}}encoding' (field type declaration has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: field type declaration has opt-out attribute -> not setting bounds
}

struct in_addr {
  int addr;
};

struct ip {
  char some_data[12];
  struct in_addr ip_src, ip_dst;
} __attribute__((packed,aligned(2)));

void test_dhclient_var_opt_out(void) {
  struct ip ip;
  struct ip ip2 __attribute__((cheri_no_subobject_bounds));
  do_stuff(&ip.ip_src); // expected-remark {{setting sub-object bounds for field 'ip_src' (pointer to 'struct in_addr') to 4 bytes}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> Bounds mode is everywhere-unsafe -> setting bounds for 'struct in_addr' addrof to 4
  do_stuff(&ip2.ip_src);
  // expected-remark@-1{{not setting bounds for 'struct ip __attribute__((cheri_no_subobject_bounds))' (base type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type has opt-out attribute -> not setting bounds
}

void test_dhclient_cast_opt_out(struct ip* ip) {
  // Check that casting the base type works fine (both before and after the *):

  do_stuff((unsigned char *)&((struct ip* __attribute__((cheri_no_subobject_bounds)))ip)->ip_src);
  // expected-remark@-1{{not setting bounds for 'struct ip * __capability __attribute__((cheri_no_subobject_bounds))' (base pointer type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base pointer type has opt-out attribute -> not setting bounds
  do_stuff((unsigned char *)&((__attribute__((cheri_no_subobject_bounds)) struct ip*)ip)->ip_src);
  // expected-remark@-1{{not setting bounds for 'struct ip __attribute__((cheri_no_subobject_bounds))' (base type has opt-out attribute)}}
  // CHECK-NEXT: subobj bounds check: got MemberExpr -> opt-out: base type has opt-out attribute -> not setting bounds
}


#ifdef NOTYET
struct TestStruct {
  int first;
  int second;
};

void expr_opt_out(struct TestStruct* t) {
  do_stuff(&t->first);
  do_stuff(__builtin_cheri_no_tighten_bounds(&t.first));
}

#ifdef __cplusplus
void do_stuff_with_ref(int&);

// TODO: what about multiple levels of nesting: e.g foo.a.b.c?
// __builtin_cheri_no_tighten_bounds will just keep the bounds

void expr_opt_out_ref(struct TestStruct& t) {
  do_stuff_with_ref(t.first);
  do_stuff_with_ref(__builtin_cheri_no_tighten_bounds(t.first));
  do_stuff_with_ref(t.first __attribute__((cheri_no_tighten_bounds)));

  // C++11 attributes:
  do_stuff_with_ref([[cheri::no_tighten_bounds]] t.first);
  do_stuff_with_ref(t.first [[cheri::no_tighten_bounds]]);
#ifdef CHECK_BAD
  do_stuff_with_ref(t.first __attribute__((cheri_no_suboject_bounds)));
  do_stuff_with_ref(t.first [[cheri::no_subobject_bounds]]);
#endif

}
#endif

#endif
