#!/bin/bash
find . ! -iname "*.sh" -type f \( -mtime 1 -o -mmin 2880 \) -execdir rename 's/^.\//.\/_/' {} \;
