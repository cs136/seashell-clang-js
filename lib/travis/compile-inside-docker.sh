#!/bin/bash


mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=/usr/emsdk_portable/emscripten/tag-1.36.1/cmake/Modules/Platform/Emscripten.cmake \
	-DCMAKE_CROSSCOMPILING=True -DCMAKE_INSTALL_PREFIX=install \
	-DLLVM_ENABLE_THREADS=0 -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_TARGET_ARCH=X86 \
	-DEMSCRIPTEN_PRELOAD=/usr/emsdk_portable/emscripten/tag-1.36.1/tools/file_packager.py \
	-DCMAKE_BUILD_TYPE=RelWithDebInfo \
	-DPATH_TO_LLVM_TBLGEN=../patch/llvm-3.9-tblgen -DCLANG_TBLGEN=../patch-clang-3.9-tblgen \
	..
make -j2 install
