#!/usr/bin/env bash

# rendera/install-dependencies.sh
#
# install packages on which rendera depends

system=`uname -s`

case $system in
    Linux)
        echo -n                                         && \
            sudo apt-get -qq update                     && \
            sudo apt-get -qq install libX11-dev         && \
            sudo apt-get -qq install libfltk1.3-dev     && \
            sudo apt-get -qq install libfontconfig1-dev && \
            sudo apt-get -qq install libglu1-mesa-dev   && \
            sudo apt-get -qq install libice-dev         && \
            sudo apt-get -qq install libjpeg-dev        && \
            sudo apt-get -qq install libjpeg62-dev      && \
            sudo apt-get -qq install libpng-dev         && \
            sudo apt-get -qq install libpng12-dev       && \
            sudo apt-get -qq install libx11-dev         && \
            sudo apt-get -qq install libxcursor-dev     && \
            sudo apt-get -qq install libxext-dev        && \
            sudo apt-get -qq install libxft-dev         && \
            sudo apt-get -qq install libxi-dev          && \
            sudo apt-get -qq install libxinerama-dev    && \
            sudo apt-get -qq install libxrender-dev     && \
            sudo apt-get -qq install libz-dev           && \
            sudo apt-get -qq install mesa-common-dev    && \
            sudo apt-get -qq install mingw32            && \
            sudo apt-get -qq install mingw32-binutils   && \
            sudo apt-get -qq install mingw32-runtime    && \
            sudo apt-get -qq install wine               && \
            sudo apt-get -qq install zlib1g-dev         && \
            echo "${BASH_SOURCE} win"
        ;;
    Darwin)
        echo -n                             && \
            brew update                     && \
            brew install libX11-dev         && \
            brew install libfltk1.3-dev     && \
            brew install libfontconfig1-dev && \
            brew install libglu1-mesa-dev   && \
            brew install libice-dev         && \
            brew install libjpeg-dev        && \
            brew install libjpeg62-dev      && \
            brew install libpng-dev         && \
            brew install libpng12-dev       && \
            brew install libx11-dev         && \
            brew install libxcursor-dev     && \
            brew install libxext-dev        && \
            brew install libxft-dev         && \
            brew install libxi-dev          && \
            brew install libxinerama-dev    && \
            brew install libxrender-dev     && \
            brew install libz-dev           && \
            brew install mesa-common-dev    && \
            brew install mingw32            && \
            brew install mingw32-binutils   && \
            brew install mingw32-runtime    && \
            brew install wine               && \
            brew install zlib1g-dev         && \
            echo "${BASH_SOURCE} win"
        ;;
    *)
        echo "unsupported system: $system"
        exit 1
esac

