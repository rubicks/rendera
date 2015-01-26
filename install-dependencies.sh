#!/usr/bin/env bash

# rendera/install-dependencies.sh
#
# install packages on which rendera depends


_system=`uname -s`

echo "\${TRAVIS_OS_NAME} == \"${TRAVIS_OS_NAME}\""
echo "\${_system} == \"${_system}\""

case "${_system}" in
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
            echo -n
        ;;
    Darwin)
        echo -n \
            && \
            brew update \
            && \
            brew install            \
                 fltk               \
                 fontconfig         \
                 wine               \
            && \
            echo -n
        ;;
    *)
        echo "unsupported system: ${_system}"
        exit 1
        ;;
esac

echo "${BASH_SOURCE} win"
