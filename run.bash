#!/usr/bin/env bash

if [[ ! -f ./bin/output.x86_64 ]]; then
    echo "Error: output doesn't exist!" >&2
    echo "Run build.bash first!" >&2
    exit 1
fi

if [[ ! -x ./bin/output.x86_64 ]]; then
    chmod +x ./bin/output.x86_64
    if [[ ! -x ./bin/output.x86_64 ]]; then
        echo "Error: cannot make output.x86_64 executable!" >&2
        exit 2
    fi
fi

./bin/output.x86_64