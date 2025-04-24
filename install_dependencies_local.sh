#!/bin/bash

# This script installs all required libraries for SPAKE2.
# Ideally, all libraries would be installed globally via sudo.
# Since we can't guarantee that, SPAKE2 is (unfortunately) hard-linked to 
# libraries build with extern/ via this script.

BASE_DIR=`pwd`
INSTALL_DIR=${BASE_DIR}/extern

# GMP                                            
GMP_INSTALL_DIR=${INSTALL_DIR}/gmp

if [ ! -d ${GMP_INSTALL_DIR} ]; then
    wget https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
    tar -xvf gmp-6.3.0.tar.xz
    cd gmp-6.3.0
    ./configure --prefix ${GMP_INSTALL_DIR}
    make -j 4
    make check
    make install
    cd ${BASE_DIR}
    rm -rf gmp-6.3.0*
fi

# libssl                                           
OPENSSL_INSTALL_DIR=${INSTALL_DIR}/openssl
if [ ! -d ${OPENSSL_INSTALL_DIR} ]; then
    wget https://www.openssl.org/source/openssl-3.3.0.tar.gz
    tar -xvf openssl-3.3.0.tar.gz
    cd openssl-3.3.0
    ./Configure --prefix=${OPENSSL_INSTALL_DIR} --openssldir=${OPENSSL_INSTALL_DIR}/ssl linux-x86_64
    make -j 4
    make install_sw
    cd ${BASE_DIR}
    rm -rf openssl-3.3.0*
fi

# libsodium                                              
SODIUM_INSTALL_DIR=${INSTALL_DIR}/sodium
if [ ! -d ${SODIUM_INSTALL_DIR} ]; then
    wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.19.tar.gz
    tar -xvf libsodium-1.0.19.tar.gz
    cd libsodium-stable
    ./configure --prefix=${SODIUM_INSTALL_DIR}
    make -j 4
    make install
    cd ${BASE_DIR}
    rm -rf libsodium*
fi

# LCOV                                          
LCOV_INSTALL_DIR=${INSTALL_DIR}/lcov
if [ ! -d ${LCOV_INSTALL_DIR} ]; then
    curl -LO https://github.com/linux-test-project/lcov/releases/download/v1.16/lcov-1.16.tar.gz
    tar -xzf lcov-1.16.tar.gz
    cd lcov-1.16
    make install PREFIX=${LCOV_INSTALL_DIR}
    cd ${BASE_DIR}
    rm -rf lcov-1.16*
fi
