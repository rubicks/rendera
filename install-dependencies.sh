#!/usr/bin/env bash

# rendera/install-dependencies.sh
#
# install packages on which rendera depends


function _install_macports()
{
    echo -n                                                                    && \
        curl -O https://distfiles.macports.org/MacPorts/MacPorts-2.3.1.tar.bz2 && \
        tar xf MacPorts-2.3.1.tar.bz2                                          && \
        cd MacPorts-2.3.1/                                                     && \
        ./configure                                                            && \
        make                                                                   && \
        sudo make install                                                      && \
        echo "win _install_macports"
}


_system=`uname -s`

echo "\${TRAVIS_OS_NAME} == \"${TRAVIS_OS_NAME}\""
echo "\${_system} == \"${_system}\""

case "${_system}" in
    Linux)
        echo "Linux begin"             && \
            sudo apt-get -y update     && \
            sudo apt-get -y autoremove && \
            sudo apt-get -y autoclean  && \
            sudo apt-get install -yy \
                 autoconf            \
                 automake            \
                 autoconf-archive    \
                 g++                 \
                 git                 \
                 libX11-dev          \
                 libfltk1.3-dev      \
                 libfontconfig1-dev  \
                 libglu1-mesa-dev    \
                 libice-dev          \
                 libjpeg-dev         \
                 libpng-dev          \
                 libtool             \
                 libx11-dev          \
                 libxcursor-dev      \
                 libxext-dev         \
                 libxft-dev          \
                 libxi-dev           \
                 libxinerama-dev     \
                 libxrender-dev      \
                 libz-dev            \
                 mesa-common-dev     \
                 mingw32             \
                 mingw32-binutils    \
                 mingw32-runtime     \
                 wine                \
                 &&                  \
            sudo apt-get -y autoremove && \
            sudo apt-get -y autoclean  && \
            echo "Linux end"
        ;;
    Darwin)
        echo -n                     && \
            brew update             && \
            brew upgrade xctool     && \
            brew install coreutils  && \
            brew install fltk       && \
            echo -n
        ;;
    *)
        echo "unsupported system: ${_system}"
        exit 1
        ;;
esac

echo "${BASH_SOURCE} win"
