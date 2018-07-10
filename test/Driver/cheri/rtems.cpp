// RUN: %clangxx %s -### -no-canonical-prefixes --target=mips64-unknown-rtems5 --sysroot=%S/platform 2>&1

// RUN: %clangxx %s -### -no-canonical-prefixes --target=mips64-unknown-rtems5 \
// RUN:     --sysroot=%S/platform 2>&1 | FileCheck %s -check-prefixes CHECK,DEFAULT
// RUN: %clangxx %s -### -no-canonical-prefixes -qrtems --target=mips64-unknown-rtems5 \
// RUN:     --sysroot=%S/platform 2>&1 | FileCheck %s -check-prefixes CHECK,QRTEMS
// CHECK: {{.*}}clang{{.*}}" "-cc1"
// CHECK: "-triple" "mips64-unknown-rtems5"
// CHECK: "-fuse-init-array"
// CHECK: "-isysroot" "[[SYSROOT:[^"]+]]"
// CHECK: "-internal-isystem" "[[SYSROOT]]{{/|\\\\}}include{{/|\\\\}}c++{{/|\\\\}}v1"
// CHECK: "-internal-externc-isystem" "[[SYSROOT]]{{/|\\\\}}include"
// CHECK: {{.*}}ld.lld{{.*}}" "-z" "rodynamic"
// CHECK: "--sysroot=[[SYSROOT]]"
// CHECK: "--build-id"
// CHECK: "-Bstatic" "-static" "-o" "a.out"
// QRTEMS: "start.o"
// DEFAULT: "crt0.o"
// CHECK-NOT: crti.o
// CHECK-NOT: crtbegin.o
// CHECK: "-L[[SYSROOT]]{{/|\\\\}}lib"
// QRTEMS: "-lrtemsbsp" "-lrtemscpu"
// CHECK: "-lc++" "-lunwind" "-lm"
// CHECK: "[[SYSROOT]]/lib/libclang_rt.builtins-mips64.a"
// CHECK: "-lc"
// CHECK-NOT: crtend.o
// CHECK-NOT: crtn.o
