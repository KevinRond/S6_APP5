#!/usr/bin/env bash
# Assumes this is run in the build/ folder.

mkdir -p output
THREADS=8

# Generate task list and pass to C++ processor with THREAD count
time ../scripts/gen_tasks.py ../data ./output/ 480 | ./asset_conv - $THREADS
