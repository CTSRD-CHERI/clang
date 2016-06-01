// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -std=c++11 -fsyntax-only -verify %s
// expected-no-diagnostics

// caused by <__mutex_base>'s inclusion of <chrono>. We just include the relevant bits...
template <class _Rep>
class duration
{
public:
    template <class _Rep2>
    explicit duration(const _Rep2& __r) { }

    static duration zero() {return duration(_Rep(0));}
};

template <class _Duration>
class time_point
{
public:
    typedef _Duration                 duration;
    duration __d_;

    time_point() : __d_(duration::zero()) {}

    template <class _Duration2>
    time_point(const time_point<_Duration2>& t)
            : __d_(t.time_since_epoch()) {}

    duration time_since_epoch() const {return __d_;}
    
    static time_point max() {return time_point();}
};

typedef time_point<duration<double> > __sys_tpf;
typedef time_point<duration<long> > __sys_tpi;

__sys_tpi t1;
__sys_tpf t2 = t1;
