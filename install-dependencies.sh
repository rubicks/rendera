#!/usr/bin/env bash

# rendera/install-dependencies.sh
#
# install packages on which rendera depends

system=`uname -s`

_pkgcmd=""

case $system in
    Linux)
        _pkgcmd="sudo apt-get -y "
        ;;
    Darwin)
        _pkgcmd="brew "
        ;;
    *)
        echo "unsupported system: $system"
        exit 1
        ;;
esac

echo -n &&
    eval ${_pkgcmd} update && \
    eval ${_pkgcmd} install \
         libX11-dev         \
         libfltk1.3-dev     \
         libfontconfig1-dev \
         libglu1-mesa-dev   \
         libice-dev         \
         libjpeg-dev        \
         libjpeg62-dev      \
         libpng-dev         \
         libpng12-dev       \
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
         wine

