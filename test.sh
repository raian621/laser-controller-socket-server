#!/usr/bin/bash

./build.sh tests
ctest --test-dir build -V --output-on-failure

# exit with prev command's return code so CI tools can detect when tests fail
exit $?