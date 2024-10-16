#!/bin/bash

find ./client ./server ./lib -name "*.cpp" | xargs clang-tidy --config-file=.clang-tidy -header-filter='(client|server|lib)/.*' --use-color
