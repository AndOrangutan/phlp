#!/bin/bash
set echo on

echo "Building everything..."

source build-engine.sh
ERRORLEVEL=$?

if [ $ERRORLEVEL -ne 0 ]
then
  echo "Error building engine:"$ERRORLEVEL && exit
fi

source build-testbed.sh
ERRORLEVEL=$?

if [ $ERRORLEVEL -ne 0 ]
then
  echo "Error building testbed:"$ERRORLEVEL && exit
fi

echo "All assemblies built successfully."
