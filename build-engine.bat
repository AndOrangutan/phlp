@ECHO OFF
REM Build script for engine
Setlocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFilenames=
FOR /R "engine" %%f IN (*.c) DO (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=engine
set compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Iengine/src -I%VULKAN_SDK%/Include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib
SET defines=-D_DEBUG -DPEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%..."
mkdir "bin"
clang %cFilenames% %compilerFlags% -o bin/%assembly%.dll %defines% %includeFlags% %linkerFlags%

