// RUN: %clang_cc1 -std=c++14 -fcoroutines-ts -verify %s -fcxx-exceptions

void no_coroutine_traits_bad_arg_await() {
  co_await a; // expected-error {{include <experimental/coroutine>}}
  // expected-error@-1 {{use of undeclared identifier 'a'}}
}

void no_coroutine_traits_bad_arg_yield() {
  co_yield a; // expected-error {{include <experimental/coroutine>}}
  // expected-error@-1 {{use of undeclared identifier 'a'}}
}


void no_coroutine_traits_bad_arg_return() {
  co_return a; // expected-error {{include <experimental/coroutine>}}
  // expected-error@-1 {{use of undeclared identifier 'a'}}
}

void no_coroutine_traits() {
  co_await 4; // expected-error {{std::experimental::coroutine_traits type was not found; include <experimental/coroutine>}}
}

namespace std {
namespace experimental {
template <typename... T>
struct coroutine_traits; // expected-note {{declared here}}
}}  // namespace std::experimental

template<typename Promise> struct coro {};
template <typename Promise, typename... Ps>
struct std::experimental::coroutine_traits<coro<Promise>, Ps...> {
  using promise_type = Promise;
};

struct awaitable {
  bool await_ready();
  template <typename F> void await_suspend(F);
  void await_resume();
} a;

struct suspend_always {
  bool await_ready() { return false; }
  template <typename F> void await_suspend(F);
  void await_resume() {}
};

struct suspend_never {
  bool await_ready() { return true; }
  template <typename F> void await_suspend(F);
  void await_resume() {}
};

void no_specialization() {
  co_await a; // expected-error {{implicit instantiation of undefined template 'std::experimental::coroutine_traits<void>'}}
}

template <typename... T>
struct std::experimental::coroutine_traits<int, T...> {};

int no_promise_type() { // expected-error {{this function cannot be a coroutine: 'std::experimental::coroutine_traits<int>' has no member named 'promise_type'}}
  co_await a;
}

template <>
struct std::experimental::coroutine_traits<double, double> { typedef int promise_type; };
double bad_promise_type(double) { // expected-error {{this function cannot be a coroutine: 'experimental::coroutine_traits<double, double>::promise_type' (aka 'int') is not a class}}
  co_await a;
}

template <>
struct std::experimental::coroutine_traits<double, int> {
  struct promise_type {};
};
double bad_promise_type_2(int) { // expected-error {{no member named 'initial_suspend'}}
  co_yield 0; // expected-error {{no member named 'yield_value' in 'std::experimental::coroutine_traits<double, int>::promise_type'}}
}

struct promise; // expected-note {{forward declaration}}
struct promise_void;
struct void_tag {};
template <typename... T>
struct std::experimental::coroutine_traits<void, T...> { using promise_type = promise; };
template <typename... T>
struct std::experimental::coroutine_traits<void, void_tag, T...>
{ using promise_type = promise_void; };

// FIXME: This diagnostic is terrible.
void undefined_promise() { // expected-error {{this function cannot be a coroutine: 'experimental::coroutine_traits<void>::promise_type' (aka 'promise') is an incomplete type}}
  co_await a;
}

struct yielded_thing { const char *p; short a, b; };

struct not_awaitable {};

struct promise {
  void get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  awaitable yield_value(int); // expected-note 2{{candidate}}
  awaitable yield_value(yielded_thing); // expected-note 2{{candidate}}
  not_awaitable yield_value(void()); // expected-note 2{{candidate}}
  void return_value(int); // expected-note 2{{here}}
};

struct promise_void {
  void get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void return_void();
};

void no_coroutine_handle() { // expected-error {{std::experimental::coroutine_handle type was not found; include <experimental/coroutine> before defining a coroutine}}
  //expected-note@-1 {{call to 'initial_suspend' implicitly required by the initial suspend point}}
  co_return 5; //expected-note {{function is a coroutine due to use of 'co_return' here}}
}

namespace std {
namespace experimental {
template <class PromiseType = void>
struct coroutine_handle {
  static coroutine_handle from_address(void *);
};
template <>
struct coroutine_handle<void> {
  template <class PromiseType>
  coroutine_handle(coroutine_handle<PromiseType>);
  static coroutine_handle from_address(void *);
};
}} // namespace std::experimental

void yield() {
  co_yield 0;
  co_yield {"foo", 1, 2};
  co_yield {1e100}; // expected-error {{cannot be narrowed}} expected-note {{explicit cast}} expected-warning {{changes value}} expected-warning {{braces around scalar}}
  co_yield {"foo", __LONG_LONG_MAX__}; // expected-error {{cannot be narrowed}} expected-note {{explicit cast}} expected-warning {{changes value}}
  co_yield {"foo"};
  co_yield "foo"; // expected-error {{no matching}}
  co_yield 1.0;
  co_yield yield; // expected-error {{no member named 'await_ready' in 'not_awaitable'}}
}

void coreturn(int n) {
  co_await a;
  if (n == 0)
    co_return 3;
  if (n == 1)
    co_return {4}; // expected-warning {{braces around scalar initializer}}
  if (n == 2)
    co_return "foo"; // expected-error {{cannot initialize a parameter of type 'int' with an lvalue of type 'const char [4]'}}
  co_return 42;
}

template <class T>
void co_await_non_dependent_arg(T) {
  co_await a;
}
template void co_await_non_dependent_arg(int);

void mixed_yield() {
  co_yield 0; // expected-note {{use of 'co_yield'}}
  return; // expected-error {{not allowed in coroutine}}
}

void mixed_yield_invalid() {
  co_yield blah; // expected-error {{use of undeclared identifier}}
  // expected-note@-1 {{function is a coroutine due to use of 'co_yield'}}
  return; // expected-error {{return statement not allowed in coroutine}}
}

template <class T>
void mixed_yield_template(T) {
  co_yield blah; // expected-error {{use of undeclared identifier}}
  // expected-note@-1 {{function is a coroutine due to use of 'co_yield'}}
  return; // expected-error {{return statement not allowed in coroutine}}
}

template <class T>
void mixed_yield_template2(T) {
  co_yield 42;
  // expected-note@-1 {{function is a coroutine due to use of 'co_yield'}}
  return; // expected-error {{return statement not allowed in coroutine}}
}

template <class T>
void mixed_yield_template3(T v) {
  co_yield blah(v);
  // expected-note@-1 {{function is a coroutine due to use of 'co_yield'}}
  return; // expected-error {{return statement not allowed in coroutine}}
}

void mixed_await() {
  co_await a; // expected-note {{use of 'co_await'}}
  return; // expected-error {{not allowed in coroutine}}
}

void mixed_await_invalid() {
  co_await 42; // expected-error {{'int' is not a structure or union}}
  // expected-note@-1 {{function is a coroutine due to use of 'co_await'}}
  return; // expected-error {{not allowed in coroutine}}
}

template <class T>
void mixed_await_template(T) {
  co_await 42;
  // expected-note@-1 {{function is a coroutine due to use of 'co_await'}}
  return; // expected-error {{not allowed in coroutine}}
}

template <class T>
void mixed_await_template2(T v) {
  co_await v; // expected-error {{'long' is not a structure or union}}
  // expected-note@-1 {{function is a coroutine due to use of 'co_await'}}
  return; // expected-error {{not allowed in coroutine}}
}
template void mixed_await_template2(long); // expected-note {{requested here}}

void only_coreturn(void_tag) {
  co_return; // OK
}

void mixed_coreturn(void_tag, bool b) {
  if (b)
    co_return; // expected-note {{use of 'co_return'}}
  else
    return; // expected-error {{not allowed in coroutine}}
}

void mixed_coreturn_invalid(bool b) {
  if (b)
    co_return; // expected-note {{use of 'co_return'}}
    // expected-error@-1 {{no member named 'return_void' in 'promise'}}
  else
    return; // expected-error {{not allowed in coroutine}}
}

template <class T>
void mixed_coreturn_template(void_tag, bool b, T v) {
  if (b)
    co_return v; // expected-note {{use of 'co_return'}}
    // expected-error@-1 {{no member named 'return_value' in 'promise_void'}}
  else
    return; // expected-error {{not allowed in coroutine}}
}
template void mixed_coreturn_template(void_tag, bool, int); // expected-note {{requested here}}

template <class T>
void mixed_coreturn_template2(bool b, T) {
  if (b)
    co_return v; // expected-note {{use of 'co_return'}}
    // expected-error@-1 {{use of undeclared identifier 'v'}}
  else
    return; // expected-error {{not allowed in coroutine}}
}

struct CtorDtor {
  CtorDtor() {
    co_yield 0; // expected-error {{'co_yield' cannot be used in a constructor}}
  }
  CtorDtor(awaitable a) {
    // The spec doesn't say this is ill-formed, but it must be.
    co_await a; // expected-error {{'co_await' cannot be used in a constructor}}
  }
  ~CtorDtor() {
    co_return 0; // expected-error {{'co_return' cannot be used in a destructor}}
  }
  // FIXME: The spec says this is ill-formed.
  void operator=(CtorDtor&) {
    co_yield 0; // expected-error {{'co_yield' cannot be used in a copy assignment operator}}
  }
  void operator=(CtorDtor const &) {
    co_yield 0; // expected-error {{'co_yield' cannot be used in a copy assignment operator}}
  }
  void operator=(CtorDtor &&) {
    co_await a; // expected-error {{'co_await' cannot be used in a move assignment operator}}
  }
  void operator=(CtorDtor const &&) {
    co_await a; // expected-error {{'co_await' cannot be used in a move assignment operator}}
  }
  void operator=(int) {
    co_await a; // OK. Not a special member
  }
};

void unevaluated() {
  decltype(co_await a); // expected-error {{cannot be used in an unevaluated context}}
  sizeof(co_await a); // expected-error {{cannot be used in an unevaluated context}}
  typeid(co_await a); // expected-error {{cannot be used in an unevaluated context}}
  decltype(co_yield a); // expected-error {{cannot be used in an unevaluated context}}
  sizeof(co_yield a); // expected-error {{cannot be used in an unevaluated context}}
  typeid(co_yield a); // expected-error {{cannot be used in an unevaluated context}}
}

constexpr auto constexpr_deduced_return_coroutine() {
  co_yield 0; // expected-error {{'co_yield' cannot be used in a constexpr function}}
  // expected-error@-1 {{'co_yield' cannot be used in a function with a deduced return type}}
}

void varargs_coroutine(const char *, ...) {
  co_await a; // expected-error {{'co_await' cannot be used in a varargs function}}
}

auto deduced_return_coroutine() {
  co_await a; // expected-error {{'co_await' cannot be used in a function with a deduced return type}}
}

struct outer {};
struct await_arg_1 {};
struct await_arg_2 {};

namespace adl_ns {
struct coawait_arg_type {};
awaitable operator co_await(coawait_arg_type);
}

namespace dependent_operator_co_await_lookup {
  template<typename T> void await_template(T t) {
    // no unqualified lookup results
    co_await t; // expected-error {{no member named 'await_ready' in 'dependent_operator_co_await_lookup::not_awaitable'}}
    // expected-error@-1 {{call to function 'operator co_await' that is neither visible in the template definition nor found by argument-dependent lookup}}
  };
  template void await_template(awaitable);

  struct indirectly_awaitable { indirectly_awaitable(outer); };
  awaitable operator co_await(indirectly_awaitable); // expected-note {{should be declared prior to}}
  template void await_template(indirectly_awaitable);

  struct not_awaitable {};
  template void await_template(not_awaitable); // expected-note {{instantiation}}

  template<typename T> void await_template_2(T t) {
    // one unqualified lookup result
    co_await t;
  };
  template void await_template(outer); // expected-note {{instantiation}}
  template void await_template_2(outer);

  struct transform_awaitable {};
  struct transformed {};

  struct transform_promise {
    typedef transform_awaitable await_arg;
    coro<transform_promise> get_return_object();
    transformed initial_suspend();
    ::adl_ns::coawait_arg_type final_suspend();
    transformed await_transform(transform_awaitable);
  };
  template <class AwaitArg>
  struct basic_promise {
    typedef AwaitArg await_arg;
    coro<basic_promise> get_return_object();
    awaitable initial_suspend();
    awaitable final_suspend();
  };

  awaitable operator co_await(await_arg_1);

  template <typename T, typename U>
  coro<T> await_template_3(U t) {
    co_await t;
  }

  template coro<basic_promise<await_arg_1>> await_template_3<basic_promise<await_arg_1>>(await_arg_1);

  template <class T, int I = 0>
  struct dependent_member {
    coro<T> mem_fn() const {
      co_await typename T::await_arg{}; // expected-error {{call to function 'operator co_await'}}}
    }
    template <class U>
    coro<T> dep_mem_fn(U t) {
      co_await t;
    }
  };

  template <>
  struct dependent_member<long> {
    // FIXME this diagnostic is terrible
    coro<transform_promise> mem_fn() const { // expected-error {{no member named 'await_ready' in 'dependent_operator_co_await_lookup::transformed'}}
      // expected-note@-1 {{call to 'initial_suspend' implicitly required by the initial suspend point}}
      // expected-note@+1 {{function is a coroutine due to use of 'co_await' here}}
      co_await transform_awaitable{};
      // expected-error@-1 {{no member named 'await_ready'}}
    }
    template <class R, class U>
    coro<R> dep_mem_fn(U u) { co_await u; }
  };

  awaitable operator co_await(await_arg_2); // expected-note {{'operator co_await' should be declared prior to the call site}}

  template struct dependent_member<basic_promise<await_arg_1>, 0>;
  template struct dependent_member<basic_promise<await_arg_2>, 0>; // expected-note {{in instantiation}}

  template <>
  coro<transform_promise>
      // FIXME this diagnostic is terrible
      dependent_member<long>::dep_mem_fn<transform_promise>(int) { // expected-error {{no member named 'await_ready' in 'dependent_operator_co_await_lookup::transformed'}}
    //expected-note@-1 {{call to 'initial_suspend' implicitly required by the initial suspend point}}
    //expected-note@+1 {{function is a coroutine due to use of 'co_await' here}}
    co_await transform_awaitable{};
    // expected-error@-1 {{no member named 'await_ready'}}
  }

  void operator co_await(transform_awaitable) = delete;
  awaitable operator co_await(transformed);

  template coro<transform_promise>
      dependent_member<long>::dep_mem_fn<transform_promise>(transform_awaitable);

  template <>
  coro<transform_promise> dependent_member<long>::dep_mem_fn<transform_promise>(long) {
    co_await transform_awaitable{};
  }

  template <>
  struct dependent_member<int> {
    coro<transform_promise> mem_fn() const {
      co_await transform_awaitable{};
    }
  };

  template coro<transform_promise> await_template_3<transform_promise>(transform_awaitable);
  template struct dependent_member<transform_promise>;
  template coro<transform_promise> dependent_member<transform_promise>::dep_mem_fn(transform_awaitable);
}

struct yield_fn_tag {};
template <>
struct std::experimental::coroutine_traits<void, yield_fn_tag> {
  struct promise_type {
    // FIXME: add an await_transform overload for functions
    awaitable yield_value(int());
    void return_value(int());

    suspend_never initial_suspend();
    suspend_never final_suspend();
    void get_return_object();
  };
};

namespace placeholder {
  awaitable f(), f(int); // expected-note 4{{possible target}}
  int g(), g(int); // expected-note 2{{candidate}}
  void x() {
    co_await f; // expected-error {{reference to overloaded function}}
  }
  void y() {
    co_yield g; // expected-error {{no matching member function for call to 'yield_value'}}
  }
  void z() {
    co_await a;
    co_return g; // expected-error {{address of overloaded function 'g' does not match required type 'int'}}
  }

  void x(yield_fn_tag) {
    co_await f; // expected-error {{reference to overloaded function}}
  }
  void y(yield_fn_tag) {
    co_yield g;
  }
  void z(yield_fn_tag) {
    co_await a;
    co_return g;
  }
}

struct bad_promise_1 {
  suspend_always initial_suspend();
  suspend_always final_suspend();
};
coro<bad_promise_1> missing_get_return_object() { // expected-error {{no member named 'get_return_object' in 'bad_promise_1'}}
  co_await a;
}

struct bad_promise_2 {
  coro<bad_promise_2> get_return_object();
  // FIXME: We shouldn't offer a typo-correction here!
  suspend_always final_suspend(); // expected-note {{here}}
};
// FIXME: This shouldn't happen twice
coro<bad_promise_2> missing_initial_suspend() { // expected-error {{no member named 'initial_suspend' in 'bad_promise_2'}}
  co_await a;
}

struct bad_promise_3 {
  coro<bad_promise_3> get_return_object();
  // FIXME: We shouldn't offer a typo-correction here!
  suspend_always initial_suspend(); // expected-note {{here}}
};
coro<bad_promise_3> missing_final_suspend() { // expected-error {{no member named 'final_suspend' in 'bad_promise_3'}}
  co_await a;
}

struct bad_promise_4 {
  coro<bad_promise_4> get_return_object();
  not_awaitable initial_suspend();
  suspend_always final_suspend();
};
// FIXME: This diagnostic is terrible.
coro<bad_promise_4> bad_initial_suspend() { // expected-error {{no member named 'await_ready' in 'not_awaitable'}}
  // expected-note@-1 {{call to 'initial_suspend' implicitly required by the initial suspend point}}
  co_await a; // expected-note {{function is a coroutine due to use of 'co_await' here}}
}

struct bad_promise_5 {
  coro<bad_promise_5> get_return_object();
  suspend_always initial_suspend();
  not_awaitable final_suspend();
};
// FIXME: This diagnostic is terrible.
coro<bad_promise_5> bad_final_suspend() { // expected-error {{no member named 'await_ready' in 'not_awaitable'}}
  // expected-note@-1 {{call to 'final_suspend' implicitly required by the final suspend point}}
  co_await a; // expected-note {{function is a coroutine due to use of 'co_await' here}}
}

struct bad_promise_6 {
  coro<bad_promise_6> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void return_void();
  void return_value(int) const;
  void return_value(int);
};
coro<bad_promise_6> bad_implicit_return() { // expected-error {{'bad_promise_6' declares both 'return_value' and 'return_void'}}
  co_await a;
}

struct bad_promise_7 {
  coro<bad_promise_7> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void return_void();
  void set_exception(int *);
};
coro<bad_promise_7> no_std_current_exc() {
  // expected-error@-1 {{you need to include <exception> before defining a coroutine that implicitly uses 'set_exception'}}
  co_await a;
}

namespace std {
int *current_exception();
}

struct bad_promise_base {
private:
  void return_void();
};
struct bad_promise_8 : bad_promise_base {
  coro<bad_promise_8> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void set_exception();                                   // expected-note {{function not viable}}
  void set_exception(int *) __attribute__((unavailable)); // expected-note {{explicitly made unavailable}}
  void set_exception(void *);                             // expected-note {{candidate function}}
};
coro<bad_promise_8> calls_set_exception() {
  // expected-error@-1 {{call to unavailable member function 'set_exception'}}
  // FIXME: also warn about private 'return_void' here. Even though building
  // the call to set_exception has already failed.
  co_await a;
}

struct bad_promise_9 {
  coro<bad_promise_9> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void await_transform(void *);                                // expected-note {{candidate}}
  awaitable await_transform(int) __attribute__((unavailable)); // expected-note {{explicitly made unavailable}}
  void return_void();
};
coro<bad_promise_9> calls_await_transform() {
  co_await 42; // expected-error {{call to unavailable member function 'await_transform'}}
  // expected-note@-1 {{call to 'await_transform' implicitly required by 'co_await' here}}
}

struct bad_promise_10 {
  coro<bad_promise_10> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  int await_transform;
  void return_void();
};
coro<bad_promise_10> bad_coawait() {
  // FIXME this diagnostic is terrible
  co_await 42; // expected-error {{called object type 'int' is not a function or function pointer}}
  // expected-note@-1 {{call to 'await_transform' implicitly required by 'co_await' here}}
}

struct call_operator {
  template <class... Args>
  awaitable operator()(Args...) const { return a; }
};
void ret_void();
struct good_promise_1 {
  coro<good_promise_1> get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  static const call_operator await_transform;
  using Fn = void (*)();
  Fn return_void = ret_void;
};
const call_operator good_promise_1::await_transform;
coro<good_promise_1> ok_static_coawait() {
  // FIXME this diagnostic is terrible
  co_await 42;
}

template<> struct std::experimental::coroutine_traits<int, int, const char**>
{ using promise_type = promise; };

int main(int, const char**) {
  co_await a; // expected-error {{'co_await' cannot be used in the 'main' function}}
}

struct good_promise_2 {
  float get_return_object();
  suspend_always initial_suspend();
  suspend_always final_suspend();
  void return_void();
};
template<> struct std::experimental::coroutine_handle<good_promise_2> {};

template<> struct std::experimental::coroutine_traits<float>
{ using promise_type = good_promise_2; };

float badly_specialized_coro_handle() { // expected-error {{std::experimental::coroutine_handle missing a member named 'from_address'}}
  //expected-note@-1 {{call to 'initial_suspend' implicitly required by the initial suspend point}}
  co_return; //expected-note {{function is a coroutine due to use of 'co_return' here}}
}
