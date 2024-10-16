#!/bin/bash

find ./client ./server ./lib -name "*.cpp" | xargs clang-tidy --config-file=.clang-tidy --use-color -header-filter='(client|server|lib)/.*'
