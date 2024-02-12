#!/bin/bash
set echo on

mkdir -p bin

# Get a list of all the .c files.
cFilenames=$(find testbed -type f -name "*.c")

# echo "Files:" $cFilenames

assembly="testbed"
compilerFlags="-g -fdeclspec -fPIC -MJ bin/testbed.o.json" 
# -fms-extensions 
# -Wall -Werror
includeFlags="-Isrc -Iengine/src/"
linkerFlags="-Lbin/ -lengine -Wl,-rpath,."
defines="-D_DEBUG -DPIMPORT"

echo "Building $assembly..."
clang $cFilenames $compilerFlags -o bin/$assembly $defines $includeFlags $linkerFlags
sed -e '1s/^/[\'$'\n''/' -e '$s/,$/\'$'\n'']/' bin/*.o.json > compile_commands.json
