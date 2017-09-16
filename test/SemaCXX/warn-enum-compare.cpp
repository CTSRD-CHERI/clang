// RUN: %clang_cc1 %s -fsyntax-only -verify

enum Foo { FooA, FooB, FooC };
enum Bar { BarD, BarE, BarF };
enum { AnonAA = 42, AnonAB = 43 };
enum { AnonBA = 44, AnonBB = 45 };

namespace name1 {
  enum Foo {F1, F2, F3};
  enum Baz {B1, B2, B3};
}

namespace name2 {
  enum Baz {B1, B2, B3};
}

using name1::Baz;
using name1::B1;
using name2::B2;
typedef name1::Foo oneFoo;
typedef name1::Foo twoFoo;
Foo getFoo();
Bar getBar();

void test () {
  Foo x = FooA;
  Bar y = BarD;
  Baz z = name1::B3;
  name1::Foo a;
  oneFoo b;
  twoFoo c;

  while (x == FooA);
  while (y == BarD);
  while (a == name1::F1);
  while (z == name1::B2);
  while (a == b);
  while (a == c);
  while (b == c);
  while (B1 == name1::B2);
  while (B2 == name2::B1);
  while (x == AnonAA); // expected-warning {{comparison of constant 'AnonAA' (42) with expression of type 'Foo' is always false}}
  while (AnonBB == y); // expected-warning {{comparison of constant 'AnonBB' (45) with expression of type 'Bar' is always false}}
  while (AnonAA == AnonAB);
  while (AnonAB == AnonBA);
  while (AnonBB == AnonAA);

  while ((x) == FooA);
  while ((y) == BarD);
  while ((a) == name1::F1);
  while (z == (name1::B2));
  while (a == (b));
  while (a == (c));
  while ((b) == (c));
  while ((B1) == (name1::B2));
  while ((B2) == (name2::B1));

  while (((x)) == FooA);
  while ((y) == (BarD));
  while ((a) == (name1::F1));
  while (z == (name1::B2));
  while ((a) == ((((b)))));
  while (((a)) == (c));
  while ((b) == (((c))));
  while ((((((B1))))) == (((name1::B2))));
  while (B2 == ((((((name2::B1)))))));

  while (B1 == B2); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while (name1::B2 == name2::B3); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while (z == name2::B2); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}

  while (((((B1)))) == B2); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while (name1::B2 == (name2::B3)); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while (z == ((((name2::B2))))); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}

  while ((((B1))) == (((B2)))); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while ((name1::B2) == (((name2::B3)))); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}
  while ((((z))) == (name2::B2)); // expected-warning  {{comparison of two values with different enumeration types ('name1::Baz' and 'name2::Baz')}}

  while (x == a); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'name1::Foo')}}
  while (x == b); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'oneFoo' (aka 'name1::Foo'))}}
  while (x == c); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'twoFoo' (aka 'name1::Foo'))}}

  while (x == y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x != y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x >= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x <= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x > y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x < y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (FooB == y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB != y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB >= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB <= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB > y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB < y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (FooB == BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB != BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB >= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB <= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB > BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB < BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (x == BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x != BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x >= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x <= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x > BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x < BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (getFoo() == y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() != y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() >= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() <= y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() > y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() < y); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (getFoo() == BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() != BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() >= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() <= BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() > BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() < BarD); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (getFoo() == getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() != getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() >= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() <= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() > getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (getFoo() < getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (FooB == getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB != getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB >= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB <= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB > getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (FooB < getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}

  while (x == getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x != getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x >= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x <= getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x > getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}
  while (x < getBar()); // expected-warning  {{comparison of two values with different enumeration types ('Foo' and 'Bar')}}



  while (y == x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y != x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y >= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y <= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y > x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y < x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (y == FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y != FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y >= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y <= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y > FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y < FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (BarD == FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD != FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD >= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD <= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD > FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD <FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (BarD == x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD != x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD >= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD <= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD < x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD > x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (y == getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y != getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y >= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y <= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y > getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (y < getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (BarD == getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD != getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD >= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD <= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD > getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (BarD < getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (getBar() == getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() != getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() >= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() <= getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() > getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() < getFoo()); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (getBar() == FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() != FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() >= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() <= FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() > FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() < FooB); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  while (getBar() == x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() != x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() >= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() <= x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() > x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}
  while (getBar() < x); // expected-warning  {{comparison of two values with different enumeration types ('Bar' and 'Foo')}}

  switch (a) {
    case name1::F1: break;
    case name1::F3: break;
    case name2::B2: break; // expected-warning {{comparison of two values with different enumeration types in switch statement ('name1::Foo' and 'name2::Baz')}}
  }

  switch (x) {
    case FooB: break;
    case FooC: break;
    case BarD: break; // expected-warning {{comparison of two values with different enumeration types in switch statement ('Foo' and 'Bar')}}
  }

  switch(getBar()) {
    case BarE: break;
    case BarF: break;
    case FooA: break; // expected-warning {{comparison of two values with different enumeration types in switch statement ('Bar' and 'Foo')}}
  }

  switch(x) {
    case AnonAA: break; // expected-warning {{case value not in enumerated type 'Foo'}}
    case FooA: break;
    case FooB: break;
    case FooC: break;
  }
}
