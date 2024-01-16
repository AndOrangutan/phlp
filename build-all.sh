#!/bin/bash
set echo on

echo "Building everything..."

pushd engine
source build.sh
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
  echo "Error building engine:"$ERRORLEVEL && exit
fi

pushd testbed
source build.sh
popd
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
  echo "Error building testbed:"$ERRORLEVEL && exit
fi

echo "All assemblies built successfully."
