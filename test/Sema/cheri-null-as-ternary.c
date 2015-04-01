// RUN: %clang_cc1 %s -fsyntax-only -verify -triple cheri-unknown-freebsd -target-abi sandbox
// expected-no-diagnostics
#define NULL (void*)0

int foo(int i);

int
bar(int x)
{
        int (*func)(int);

        func = x == 0 ? foo : NULL;

        if (func != NULL)
                return (func(x));
        else
                return (-1);
}
