#!/bin/bash
find . -type f -user $(id -u "$USER") | wc -l
