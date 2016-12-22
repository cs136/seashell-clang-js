#!/bin/bash
LLVM_VERSION=3.9
ARCH=X86

cd /usr/src
mkdir build
cd build

echo "Installing ccache..."
apt-get install -y ccache
set PATH="/usr/lib/ccache;$PATH"

echo "Installing npm/nodeunit"
apt-get install -y npm
npm install -g nodeunit
ln -s `which nodejs` /usr/local/bin/node

echo "Linking emcc/em++ to ccache."
ln -s `which ccache` /usr/lib/ccache/emcc
ln -s `which ccache` /usr/lib/ccache/em++

echo "Starting build in docker container..."
/emsdk_portable/emscripten/tag-${EMSCRIPTEN_VERSION}/emcmake cmake \
  -DCMAKE_BUILD_TYPE=MinSizeRel \
  -DCMAKE_C_COMPILER=/usr/lib/ccache/emcc -DCMAKE_CXX_COMPILER=/usr/lib/ccache/em++ \
	-DCMAKE_CROSSCOMPILING=True -DCMAKE_INSTALL_PREFIX=install \
	-DLLVM_ENABLE_THREADS=0 -DLLVM_TARGETS_TO_BUILD=${ARCH} -DLLVM_TARGET_ARCH=${ARCH} \
	-DEMSCRIPTEN_PRELOAD=/emsdk_portable/emscripten/tag-${EMSCRIPTEN_VERSION}/tools/file_packager.py \
	-DLLVM_TABLEGEN=/usr/src/patch/llvm-${LLVM_VERSION}-tblgen -DCLANG_TABLEGEN=/usr/src/patch/clang-${LLVM_VERSION}-tblgen \
	-GNinja \
	/usr/src

ninja -j2
ninja -j2 test
ninja -j2 install
