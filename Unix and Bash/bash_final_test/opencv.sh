#!/bin/bash


while read f; do
    incs="$(sed -n -e '/^#\s*include\s*<opencv2\/.*$/ p' "$f")"
    sed -i -e '/^#\s*include\s*<opencv2\/.*$/ d' "$f"
    sed -i -e '1i\#pragma warning(push, 0)\n'"${incs}"'\npragma warning(pop)' "$f"
done < <(find . \( -name "*.cpp" -o  -name "*.hpp" \) )
