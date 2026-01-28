#!/usr/bin/env sh
set -e

echo "Building tests..."
gcc -std=c11 -Wextra -Wall -I../src \
  -o run_tests \
  run_tests.c \
  ../src/Combinaisons.c \
  ../src/Players.c

echo "Running tests..."
./run_tests
