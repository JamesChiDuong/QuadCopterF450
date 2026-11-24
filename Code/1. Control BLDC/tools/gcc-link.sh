#!/bin/sh
# Link through arm-none-eabi-ld directly, with exactly the arguments gcc
# would pass to collect2 (obtained with "gcc -###").
#
# Used by "make DIRECT_LD=1" on Windows machines where an application control
# policy (e.g. Smart App Control) refuses to run collect2.exe but allows ld.exe.
# collect2 is only a wrapper around ld; LTO is not used, so the LTO plugin
# arguments are dropped.
#
# usage: tools/gcc-link.sh arm-none-eabi-gcc <normal gcc link arguments>

cmd=$("$@" -### 2>&1 | grep -E 'collect2(\.exe)?"' | head -1)
if [ -z "$cmd" ]; then
    echo "gcc-link.sh: could not obtain the link command from gcc -###" >&2
    exit 1
fi

ld="${1%gcc}ld"
cmd=$(printf '%s\n' "$cmd" | sed -E \
    -e "s#^ *\"[^\"]*collect2(\.exe)?\"#\"$ld\"#" \
    -e 's# -plugin "[^"]*"##' \
    -e 's# "-plugin-opt=[^"]*"##g')

eval "$cmd"
