// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi sandbox -fsyntax-only -verify %s
// expected-no-diagnostics
// Reduced from <locale>
class money_base {
  public:
    struct pattern;
};

class moneypunct : public money_base {
  public:
    virtual pattern do_pos_format();
};

class moneypunct_byname : public moneypunct {
  public:
    typedef money_base::pattern  pattern;
    virtual pattern do_pos_format();
};
