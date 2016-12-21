#!/bin/bash
LLVM_VERSION=3.9
ARCH=X86

cd /usr/src
mkdir build
cd build

echo "Starting build in docker container..."
cmake -DCMAKE_TOOLCHAIN_FILE=/usr/emsdk_portable/emscripten/tag-${EMSCRIPTEN_VERSION}/cmake/Modules/Platform/Emscripten.cmake \
	-DCMAKE_CROSSCOMPILING=True -DCMAKE_INSTALL_PREFIX=install \
	-DLLVM_ENABLE_THREADS=0 -DLLVM_TARGETS_TO_BUILD=${ARCH} -DLLVM_TARGET_ARCH=${ARCH} \
	-DEMSCRIPTEN_PRELOAD=/usr/emsdk_portable/emscripten/tag-${EMSCRIPTEN_VERSION}/tools/file_packager.py \
	-DCMAKE_BUILD_TYPE=RelWithDebInfo \
	-DPATH_TO_LLVM_TBLGEN=../patch/llvm-${LLVM_VERSION}-tblgen -DCLANG_TBLGEN=../patch-clang-${LLVM_VERSION}-tblgen \
	/usr/src

make -j2 install
