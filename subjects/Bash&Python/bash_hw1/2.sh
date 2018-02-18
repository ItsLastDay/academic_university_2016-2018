#!/bin/bash
cat input.txt | sort | uniq | shuf | cat -n
