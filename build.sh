#!/bin/bash

CURRENT_DIR=$(pwd)
BUILD_DIR=${CURRENT_DIR}/build/
CPU_NUM=$(cat /proc/cpuinfo | grep "physical id" | wc -l)

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
pushd ${BUILD_DIR}

cmake ${CURRENT_DIR} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-std=c++11 -g"\
                     -DCMAKE_INCLUDE_PATH="${BUILD_DIR}"

make -j${CPU_NUM}
make install
popd