#!/usr/bin/env bash

#    This file is part of SDL Testing.
#
#    SDL Testing is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Affero General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    SDL Testing is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Affero General Public License for more details.
#
#    You should have received a copy of the GNU Affero General Public License
#    along with SDL Testing.  If not, see <https://www.gnu.org/licenses/>.

if [[ $1 =~ ^-{1,2}([Vv]([Ee][Rr][Bb][Oo][Ss][Ee])?)$ ]]; then
    VERBOSE=1
    shift
fi

if [[ $1 =~ ^-{1,2}([Dd]([Ee][Bb][Uu][Gg])?)$ ]]; then
    DEBUG=1
    shift
elif [[ $1 =~ ^-{1,2}([Rr]([Ee][Ll][Ee][Aa][Ss][Ee])?)$ ]]; then
    RELEASE=1
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

if [[ $DEBUG ]]; then
    valid_sources+=( "-O0" )
elif [[ $RELEASE ]]; then
    valid_sources+=( "-O3" )
fi

# Clang compile
[[ $VERBOSE ]] && echo "Command: clang++ -Wall -Wextra -std=c++23 ${valid_sources[*]} -lSDL3 -o bin/output.x86_64 $*"
clang++ -Wall -Wextra -std=c++23 "${valid_sources[@]}" -lSDL3 -lSDL3_image -o bin/output.x86_64 "$@"