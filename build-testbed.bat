REM Build script for testbed
@ECHO OFF
Setlocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFilenames=
FOR /R "testbed" %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=testbed
set compilerFlags=-g -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc -Iengine/src/
SET linkerFlags=-Lbin/ -lengine.lib
SET defines=-D_DEBUG -DPIMPORT

ECHO "Building %assembly%..."
mkdir "bin"
clang %cFilenames% %compilerFlags% -o bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

