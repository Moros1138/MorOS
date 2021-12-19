#!/bin/sh
MOROS_ROOT=$(pwd)

switch()
{
    cp $MOROS_ROOT/tools/c_cpp_properties.json.$1  $MOROS_ROOT/.vscode/c_cpp_properties.json
}