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
COMMAND=$1
shift

shopt -s extglob;
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        +(wall|ground|bunny|bunny|frog|horse|maxplanck|sphere|torus|dragon|happy|lucy|moai|tetrahedron))
        INPUT_FILE="models/${key}.ply"
        ;;
        +(small_museum))
        INPUT_FILE="resources/${key}.txt"
        ;;
        --LOD|--calc-LOD)
        if [[ $INPUT_FILE == *".txt" ]]; then
            INPUT_FILE=$DEFAULT_MODEL
        fi
        LOD_FLAG="--calc-LOD"
        LOD_LEVEL=4
        ;;
        0|1|2|3|4)
        LOD_LEVEL=$1
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
    cmake --build . || { exit 1; }
    ;;
"clean")
    rm -rf ./build/*
    ;;
esac

# run
case $COMMAND in
""|"run"|"build")
    ./BaseCode $INPUT_FILE $LOD_FLAG $LOD_LEVEL
    ;;
esac
