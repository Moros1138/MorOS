#!/bin/sh
MOROS_ROOT=$(pwd)

switch()
{
    cp $MOROS_ROOT/tools/c_cpp_properties.json.$1  $MOROS_ROOT/.vscode/c_cpp_properties.json
    cd $MOROS_ROOT/$1
}

build()
{
    BACKUP_PWD=$(pwd)
    
    if [ "$1" == "kernel" ]; then
        cd $MOROS_ROOT/kernel
        make clean && clear && make run
    fi

    if [ "$1" == "exp" ]; then
        cd $MOROS_ROOT/exp
        make clean && clear && make && bin/experiments
    fi
    
    cd $BACKUP_PWD
}
