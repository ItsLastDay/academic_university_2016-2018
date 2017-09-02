#!/bin/bash
ping "$(cat ip.txt)" > res.txt 2> err.txt
