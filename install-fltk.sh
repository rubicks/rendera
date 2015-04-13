#!/usr/bin/env bash

# rendera/install-fltk.sh
#
# compile fltk from source and install

_repo="https://github.com/IngwiePhoenix/FLTK.git"

echo -n                                          && \
    cd $(mktemp -d)                              && \
    git clone -v --depth=10 ${_repo}             && \
    cd FLTK                                      && \
    make makeinclude                             && \
    ./configure --host=$HOST --prefix=/usr/$HOST && \
    make                                         && \
    sudo make install

exit ${?}
