#!/bin/bash

CURRENT_DIR=$(pwd)
BUILD_DIR=${CURRENT_DIR}/build/

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
pushd ${BUILD_DIR}

cmake ${CURRENT_DIR} -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-std=c++11 -s"
make
make install
popd