// Crash reproducer for clang version 5.0.0
// Driver args: "-g" "-fintegrated-as" "--target=cheri-unknown-freebsd" "-msoft-float" "--sysroot=/home/alr48/cheri/build-postmerge/cheribsd-obj-256/mips.mips64/exports/users/alr48/sources/cheribsd-postmerge/tmp" "-Wno-deprecated-declarations" "-mabi=purecap" "-mxgot" "-fpic" "-O2" "-pipe" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/include" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/locale" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libcheri" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libmalloc_simple" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/tzcode/stdtime" "-mxgot" "-mllvm" "-mxmxgot" "-G" "0" "-mbig-endian" "-msoft-float" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/include" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips/softfloat" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/locale" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/regex" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/softfloat" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/string" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/stdlib" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/stdtime" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../include" "-Wno-cast-align" "-D" "FORCE_C_LOCALE" "-D" "FORCE_UTC_TZ" "-O2" "-ftls-model=local-exec" "-D" "__LP64__=1" "-g" "-MD" "-MF" ".depend.misc.o" "-MT" "misc.o" "-std=gnu99" "-Wsystem-headers" "-Wall" "-Wno-format-y2k" "-W" "-Wno-unused-parameter" "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wpointer-arith" "-Wreturn-type" "-Wcast-qual" "-Wwrite-strings" "-Wswitch" "-Wshadow" "-Wunused-parameter" "-Wcast-align" "-Wchar-subscripts" "-Winline" "-Wnested-externs" "-Wredundant-decls" "-Wold-style-definition" "-Wno-pointer-sign" "-Wmissing-variable-declarations" "-Wthread-safety" "-Wno-empty-body" "-Wno-string-plus-int" "-Wno-unused-const-variable" "-Qunused-arguments" "-Wno-sign-compare" "-Wno-sign-compare" "-c" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa/misc.c" "-o" "misc.o"
// Original command:  "/home/alr48/cheri/output-postmerge/sdk256/bin/clang-5.0" "-cc1" "-triple" "cheri-unknown-freebsd" "-emit-obj" "-disable-free" "-main-file-name" "misc.c" "-mrelocation-model" "pic" "-pic-level" "1" "-mthread-model" "posix" "-mdisable-fp-elim" "-masm-verbose" "-mconstructor-aliases" "-target-cpu" "cheri" "-target-feature" "+soft-float" "-target-abi" "purecap" "-Wmips-cheri-prototypes" "-msoft-float" "-mfloat-abi" "soft" "-mllvm" "-mxgot" "-mllvm" "-mips-ssection-threshold=0" "-dwarf-column-info" "-debug-info-kind=standalone" "-dwarf-version=2" "-debugger-tuning=gdb" "-coverage-notes-file" "/home/alr48/cheri/build-postmerge/cheribsd-obj-256/mips.mips64/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/misc.gcno" "-resource-dir" "/home/alr48/cheri/output-postmerge/sdk256/lib/clang/5.0.0" "-dependency-file" ".depend.misc.o" "-sys-header-deps" "-MT" "misc.o" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/include" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/locale" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libcheri" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libmalloc_simple" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/tzcode/stdtime" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/include" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/mips/softfloat" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/locale" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/regex" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/softfloat" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/string" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/stdlib" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../libc/stdtime" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa" "-I" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../include" "-D" "FORCE_C_LOCALE" "-D" "FORCE_UTC_TZ" "-D" "__LP64__=1" "-isysroot" "/home/alr48/cheri/build-postmerge/cheribsd-obj-256/mips.mips64/exports/users/alr48/sources/cheribsd-postmerge/tmp" "-O2" "-Wno-deprecated-declarations" "-Wno-cast-align" "-Wsystem-headers" "-Wall" "-Wno-format-y2k" "-W" "-Wno-unused-parameter" "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wpointer-arith" "-Wreturn-type" "-Wcast-qual" "-Wwrite-strings" "-Wswitch" "-Wshadow" "-Wunused-parameter" "-Wcast-align" "-Wchar-subscripts" "-Winline" "-Wnested-externs" "-Wredundant-decls" "-Wold-style-definition" "-Wno-pointer-sign" "-Wmissing-variable-declarations" "-Wthread-safety" "-Wno-empty-body" "-Wno-string-plus-int" "-Wno-unused-const-variable" "-Wno-sign-compare" "-Wno-sign-compare" "-std=gnu99" "-fconst-strings" "-fdebug-compilation-dir" "/home/alr48/cheri/build-postmerge/cheribsd-obj-256/mips.mips64/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri" "-ferror-limit" "19" "-fmessage-length" "0" "-ftls-model=local-exec" "-fobjc-runtime=gnustep" "-fdiagnostics-show-option" "-vectorize-loops" "-vectorize-slp" "-cheri-linker" "-mllvm" "-mxmxgot" "-o" "misc.o" "-x" "c" "/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/../../contrib/gdtoa/misc.c"

// RUN: %clang_cc1 -triple "cheri-unknown-freebsd" "-emit-obj" "-disable-free" "-main-file-name" "misc.c" "-mrelocation-model" "pic" "-pic-level" "1" "-mthread-model" "posix" "-mdisable-fp-elim" "-masm-verbose" "-mconstructor-aliases" "-target-feature" "+soft-float" "-target-abi" "purecap" "-Wmips-cheri-prototypes" "-msoft-float" "-mfloat-abi" "soft" "-mllvm" "-mxgot" "-mllvm" "-mips-ssection-threshold=0" "-dwarf-column-info" "-debug-info-kind=standalone" "-dwarf-version=2" "-debugger-tuning=gdb" "-coverage-notes-file" "/home/alr48/cheri/build-postmerge/cheribsd-obj-256/mips.mips64/exports/users/alr48/sources/cheribsd-postmerge/lib/libc_cheri/misc.gcno" "-sys-header-deps" "-D" "FORCE_C_LOCALE" "-D" "FORCE_UTC_TZ" "-D" "__LP64__=1" "-O2" "-Wno-deprecated-declarations" "-Wno-cast-align" "-Wsystem-headers" "-Wall" "-Wno-format-y2k" "-W" "-Wno-unused-parameter" "-Wstrict-prototypes" "-Wmissing-prototypes" "-Wpointer-arith" "-Wreturn-type" "-Wcast-qual" "-Wwrite-strings" "-Wswitch" "-Wshadow" "-Wunused-parameter" "-Wcast-align" "-Wchar-subscripts" "-Winline" "-Wnested-externs" "-Wredundant-decls" "-Wold-style-definition" "-Wno-pointer-sign" "-Wmissing-variable-declarations" "-Wthread-safety" "-Wno-empty-body" "-Wno-string-plus-int" "-Wno-unused-const-variable" "-Wno-sign-compare" "-Wno-sign-compare" "-std=gnu99" "-fconst-strings" "-ferror-limit" "19" "-fmessage-length" "0" "-ftls-model=local-exec" "-fobjc-runtime=gnustep" "-fdiagnostics-show-option" "-vectorize-loops" "-vectorize-slp" "-cheri-linker" "-mllvm" "-mxmxgot" "-o" "/dev/null" "-x" "c" %s

// Compiling contrib/gdtoa/misc.c used to crash

#define	Long	int
#ifndef ULong
typedef unsigned Long ULong;
#endif
#ifndef UShort
typedef unsigned short UShort;
#endif

#ifndef ANSI
#ifdef KR_headers
#define ANSI(x) ()
#define Void /*nothing*/
#else
#define ANSI(x) x
#define Void void
#endif
#endif /* ANSI */

#ifndef CONST
#ifdef KR_headers
#define CONST /* blank */
#else
#define CONST const
#endif
#endif /* CONST */

 struct
Bigint {
	struct Bigint *next;
	int k, maxwds, sign, wds;
	ULong x[1];
	};

 typedef struct Bigint Bigint;

#define	cmp		__cmp_D2A
 extern int cmp ANSI((Bigint*, Bigint*));
 int
cmp
#ifdef KR_headers
	(a, b) Bigint *a, *b;
#else
	(Bigint *a, Bigint *b)
#endif
{
	ULong *xa, *xa0, *xb, *xb0;
	int i, j;

	i = a->wds;
	j = b->wds;
#ifdef DEBUG
	if (i > 1 && !a->x[i-1])
		Bug("cmp called with a->x[a->wds-1] == 0");
	if (j > 1 && !b->x[j-1])
		Bug("cmp called with b->x[b->wds-1] == 0");
#endif
	if (i -= j)
		return i;
	xa0 = a->x;
	xa = xa0 + j;
	xb0 = b->x;
	xb = xb0 + j;
	for(;;) {
		if (*--xa != *--xb)
			return *xa < *xb ? -1 : 1;
		if (xa <= xa0)
			break;
		}
	return 0;
	}
