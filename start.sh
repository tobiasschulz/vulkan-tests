#!/bin/bash

build() {
    cd $HOME/src/VulkanSDK/*; . setup-env.sh; cd $OLDPWD

    echo $PATH

    test -d build || mkdir build
    (
        cd build && \
        (
            for x in ../src/shaders/*
            do
                glslangValidator -V "${x}" -o "$(basename "${x}")".spv
            done
        ) && \
        cmake .. && \
        make
    )
}

rebuild() {
    rm -rf build
}

run() {
    export N=$1
    shift
    (
        cd build && \
        ./vulkantest_$N "$@"
    )
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
