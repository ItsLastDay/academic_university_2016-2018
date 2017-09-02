#!/bin/bash
<<< "$1" grep -P "^(xx+)(\1)+$"
