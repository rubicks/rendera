#!/usr/bin/env bash
#
# rendera/build-o-matic.sh


# if [[ "Darwin" == $(uname -s) ]]
# then
#     export PATH="/usr/local/opt/coreutils/libexec/gnubin:$PATH"
#     export MANPATH="/usr/local/opt/coreutils/libexec/gnuman:$MANPATH"
#     echo "\${PATH}    == \"${PATH}\""
#     echo "\${MANPATH} == \"${MANPATH}\""
# fi

echo -n                                                             && \
    export PROJECT_DIR=$(dirname $(readlink -f ${BASH_SOURCE}))     && \
    export RENDERA_BUILD_DIR=$(mktemp -t -d rendera-build-XXXXXX)   && \
    export RENDERA_PREFIX_DIR=$(mktemp -t -d rendera-prefix-XXXXXX) && \
    echo -n                        && \
    cd ${RENDERA_BUILD_DIR}        && \
    autoreconf -ivf ${PROJECT_DIR} && \
    ${PROJECT_DIR}/configure \
                  --host=${HOST} \
                  --prefix=${RENDERA_PREFIX_DIR} && \
    make           && \
    make check     && \
    make distcheck && \
    make install   && \
    echo && echo "big win"

#     man -M ${RENDERA_PREFIX_DIR}/share/man -t rendera | ps2ascii -  && \
    # export PATH="${RENDERA_PREFIX_DIR}/bin:${PATH}"                 && \
    # rendera --help                                                  && \
    # rendera --version                                               && \
