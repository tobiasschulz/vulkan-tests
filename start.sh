#!/bin/bash

build() {
    bash $HOME/src/VulkanSDK/*/setup-env.sh

    rm -rf build && \
    mkdir build && \
    (
        cd build && \
        cmake .. && \
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
    build && (
        shift
        run "$@"
    )
    ;;
esac
