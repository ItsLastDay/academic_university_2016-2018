#!/bin/bash
# API Limit: 150 queries per minute.
parallel -j$(nproc) -i bash -c 'curl -s "http://ip-api.com/json/{}" | jq .city' -- $(cat ips.txt) | sort -u
