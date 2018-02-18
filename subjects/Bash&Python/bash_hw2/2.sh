#!/bin/bash
cmd_name=${1?"Need to specify command name"}

cat ~/.bash_history | grep -c "^${cmd_name}"
