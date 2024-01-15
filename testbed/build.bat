REM Build script for testbed
@ECHO OFF
Setlocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFilenames=
FOR /R %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=testbed
set compilerFlags=-g
REM -Wall -Werror
SET includeFlags=-Isrc -I../engine/src/
SET linkerFlags=-L../bin/ -lengine.lib
SET defines=-D_DEBUG -DPIMPORT

ECHO "Building %assembly%..."
clang %cFilenames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

