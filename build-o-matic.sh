#!/usr/bin/env bash
#
# rendera/build-o-matic.sh

echo -n                                                                     && \
    export PROJECT_DIR=$(dirname $(readlink --canonicalize ${BASH_SOURCE})) && \
    mkdir -vp ${PROJECT_DIR}/m4                                             && \
    export RENDERA_BUILD_DIR=$(mktemp -t -d rendera-build-XXXXXX)           && \
    cd ${RENDERA_BUILD_DIR}                                                 && \
    autoreconf -ivf ${PROJECT_DIR}                                          && \
    export RENDERA_PREFIX_DIR=$(mktemp -t -d rendera-prefix-XXXXXX)         && \
    ${PROJECT_DIR}/configure --prefix=${RENDERA_PREFIX_DIR}                 && \
    make                                                                    && \
    make check                                                              && \
    make distcheck                                                          && \
    make install                                                            && \
    export PATH="${RENDERA_PREFIX_DIR}/bin:${PATH}"                         && \
    rendera --help                                                          && \
    rendera --version                                                       && \
    man -M ${RENDERA_PREFIX_DIR}/share/man -t rendera | ps2ascii -          && \
    echo && echo "big win"
