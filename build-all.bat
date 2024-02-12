REM Build everything
@ECHO OFF

ECHO "Building All..."

PUSHD engine
CALL build.bat
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
POPD

PUSHD testbed
CALL build.bat
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
POPD

echo "All assemblies built successfully!
