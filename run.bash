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

./bin/output.x86_64 "$@"