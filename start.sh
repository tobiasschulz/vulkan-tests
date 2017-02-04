#!/bin/bash

build() {
    rm -rf build && \
    mkdir build && \
    cd build && (
        cmake ..
        make
    )
}

run() {
    export N=$1
    shift
    build/vulkantest_$N "$@"
}

case $1 in
"build")
    build
    ;;
"run")
    shift
    run "$@"
    ;;
esac
