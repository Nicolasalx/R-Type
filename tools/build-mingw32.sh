#!/bin/bash
CI_FLAG="OFF"
while getopts "c:" opt; do
  case $opt in
    c)
      CI_FLAG=$OPTARG
      ;;
    *)
      echo "Invalid option: -$opt"
      exit 1
      ;;
  esac
done

mkdir -p build-mingw32

cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/mingw32-toolchain.cmake \
      -S .                                                   \
      -B build-mingw32                                       \
      -DCI=${CI_FLAG}

cmake --build build-mingw32 --config Release
