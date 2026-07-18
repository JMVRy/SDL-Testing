#!/usr/bin/env bash

# Get source files
sources=( ./src/**.cpp )

for file in "${sources[@]}"; do
    if [[ ! -f "$file" ]]; then
        # something went wrong with the glob
        echo "Uh oh, no scripts!" >&2
        echo "Sources: ${sources[*]}"
        exit 1
    fi
done

# Clang compile
clang++ -O3 -Wall -Wextra -Werror -std=c++23 "${sources[@]}" -o bin/output.x86_64