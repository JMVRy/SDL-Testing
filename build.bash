#!/usr/bin/env bash

if [[ $1 =~ ^-{1,2}([Vv]([Ee][Rr][Bb][Oo][Ss][Ee])?)$ ]]; then
    VERBOSE=1
    shift
fi

# Get source files
sources=( ./src/**.cpp )
valid_sources=()

for file in "${sources[@]}"; do
    if [[ ! -f "$file" ]]; then
        # something went wrong with the glob
        echo "File doesn't exist!" >&2
        echo "File: $file" >&2
        continue
    fi

    valid_sources+=( "$file" )
done

# Clang compile
[[ $VERBOSE ]] && echo "Command: clang++ -O3 -Wall -Wextra -std=c++23 ${valid_sources[*]} -lSDL3 -o bin/output.x86_64 $*"
clang++ -O3 -Wall -Wextra -std=c++23 "${valid_sources[@]}" -lSDL3 -lSDL3_image -o bin/output.x86_64 "$@"