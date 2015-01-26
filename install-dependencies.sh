#!/usr/bin/env bash

# rendera/install-dependencies.sh
#
# install packages on which rendera depends

system=`uname -s`

case $system in
    Linux)
        echo -n && \
            sudo apt-get -y update && \
            sudo apt-get -y install \
                 libX11-dev         \
                 libfltk1.3-dev     \
                 libfontconfig1-dev \
                 libglu1-mesa-dev   \
                 libice-dev         \
                 libjpeg-dev        \
                 libjpeg62-dev      \
                 libpng-dev         \
                 libpng12-dev       \
                 libx11-dev         \
                 libxcursor-dev     \
                 libxext-dev        \
                 libxft-dev         \
                 libxi-dev          \
                 libxinerama-dev    \
                 libxrender-dev     \
                 libz-dev           \
                 mesa-common-dev    \
                 mingw32            \
                 mingw32-binutils   \
                 mingw32-runtime    \
                 wine               \
            && \
            echo "${BASH_SOURCE} win"
        ;;
    Darwin)
        echo -n && \
            brew update && \
            brew install            \
                 fltk               \
                 libfontconfig1-dev \
                 libglu1-mesa-dev   \
                 libice-dev         \
                 libjpeg-dev        \
                 libjpeg62-dev      \
                 libpng-dev         \
                 libpng12-dev       \
                 libx11-dev         \
                 libxcursor-dev     \
                 libxext-dev        \
                 libxft-dev         \
                 libxi-dev          \
                 libxinerama-dev    \
                 libxrender-dev     \
                 libz-dev           \
                 mesa-common-dev    \
                 mingw32            \
                 mingw32-binutils   \
                 mingw32-runtime    \
                 wine               \
            && \
            echo "${BASH_SOURCE} win"
        ;;
    *)
        echo "unsupported system: ${system}"
        exit 1
        ;;
esac
