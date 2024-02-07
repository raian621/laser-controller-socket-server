#!/usr/bin/bash

printUsage() {
  echo "Usage: $0 [mode]"
  echo "modes: prod, tests, no_pgpio"
}

mode=$1
if [[ $mode = "" ]]; then
  mode="prod"
fi

case $mode in
  prod)
  cmake -DBUILD_TESTING=OFF -DMOCK_PIGPIO=OFF -S . -B build
  ;;

  tests)
  cmake -DBUILD_TESTING=ON -DMOCK_PIGPIO=ON -S . -B build
  ;;

  no_pigpio)
  cmake -DMOCK_PIGPIO=ON -S . -B build
  ;;

  *)
  printUsage
  exit 1
  ;;
esac

status_code=$?
if [ $status_code -ne 0 ]; then
  exit $status_code
fi

cmake --build build
exit $?
