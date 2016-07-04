// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// Reduced from <__locale>
template <class T> class string;

template <class T>
class collate {
  public:
    typedef string<T> string_type;
    virtual string_type do_transform();
};

template <class _CharT> class collate_byname;

template <>
class collate_byname<char> : public collate<char> {
  public:
    typedef string<char> string_type;
    virtual string_type do_transform();
};
