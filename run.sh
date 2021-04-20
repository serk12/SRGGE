#!/bin/bash
# -*- ENCODING: UTF-8 -*-

# use .env file
set -a
    [ -f .env ] && . .env
set +a
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

MODEL="resources/small_museum.txt"
LOD_FLAG=""
COMMAND=$1
shift

shopt -s extglob;
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        +(wall|ground|bunny|bunny|frog|horse|maxplanck|sphere|torus|dragon|happy|lucy|moai|tetrahedron))
        MODEL="models/${key}.ply"
        ;;
        +(small_museum))
        MODEL="resources/${key}.txt"
        ;;
        --LOD|--calc-LOD)
        LOD_FLAG="--calc-LOD"
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
    ./BaseCode $MODEL $LOD_FLAG
    ;;
esac
