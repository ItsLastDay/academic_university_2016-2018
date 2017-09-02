#!/bin/bash
grep -oP '^\s*+#\s*+include\s*+((?<bracket><)|")\K.*?(?=(?(<bracket>)>|"))' main.c
