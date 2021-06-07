#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# use .env file
set -a
    [ -f .env ] && . .env
set +a
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

DEFAULT_TAILMAP="resources/small_museum.txt"
DEFAULT_MODEL="models/bunny.ply"

INPUT_FILE=$DEFAULT_TAILMAP
LOD_FLAG=""
LOD_LEVEL=""
CULLING_FLAG=""
CULLING_LEVEL=""
CONFIG="Debug"
COMMAND=$1
shift

shopt -s extglob;
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        +(cosa|wall|ground|bunny|bunny|frog|horse|maxplanck|sphere|torus|dragon|happy|lucy|moai|tetrahedron))
        INPUT_FILE="models/${key}.ply"
        DEFAULT_MODEL="models/${key}.ply"
        ;;
        +(small_museum|big_grid))
        INPUT_FILE="resources/${key}.txt"
        DEFAULT_TAILMAP="resources/${key}.txt"
        ;;
        --LOD|--calc-LOD)
        if [[ $INPUT_FILE == *".txt" ]]; then
            INPUT_FILE=$DEFAULT_MODEL
        fi
        LOD_FLAG="--calc-LOD"
        shift
        LOD_LEVEL=$1
        ;;
        --CULLING|-c|--cull)
        CULLING_FLAG="--CULLING"
        shift
        CULLING_LEVEL=$1
        ;;
        --RELEASE)
        CONFIG="Release"
        ;;
    esac
    shift
done
shopt -u extglob;

# pre-build
case $COMMAND in
""|"run"|"build")
    cd ./build
    ;;
esac

# build
case $COMMAND in
"models")
    ls -l --block-size=M ../models/*.ply
    ;;
"init")
    mkdir -p build
    cd ./build
    cmake ../
    cd "$parent_path"
    $0 build
    ;;
"build")
    cmake --build . --config $CONFIG  || { exit 1; }
    ;;
"clean")
    rm -rf ./build/*
    ;;
esac

# run
case $COMMAND in
""|"run"|"build")
    echo $INPUT_FILE
    ./BaseCode $INPUT_FILE $LOD_FLAG $LOD_LEVEL $CULLING_FLAG $CULLING_LEVEL
    ;;
esac
