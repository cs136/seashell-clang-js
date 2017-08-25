#!/bin/bash -e
LLVM_VERSION=3.9
ARCH=X86

apt-get update
echo "Installing ccache and unzip"
apt-get install -y ccache unzip
set PATH="/usr/lib/ccache:$PATH"

echo "Installing npm/nodeunit"
npm install -g nodeunit

echo "Installing ninja"
wget https://github.com/ninja-build/ninja/releases/download/v1.7.2/ninja-linux.zip
unzip ninja-linux.zip
cp ninja /usr/local/bin/ninja

echo "Linking emcc/em++ to ccache."
ln -s `which ccache` /usr/lib/ccache/emcc
ln -s `which ccache` /usr/lib/ccache/em++

cd /usr/src
if ! [ -d build ]; then
  mkdir build
fi
cd build

echo "Starting build in docker container..."
emcmake cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=/usr/lib/ccache/emcc -DCMAKE_CXX_COMPILER=/usr/lib/ccache/em++ \
	-DCMAKE_CROSSCOMPILING=True -DCMAKE_INSTALL_PREFIX=install \
  -DLLVM_ENABLE_THREADS=0 -DLLVM_TARGETS_TO_BUILD=${ARCH} -DLLVM_TARGET_ARCH=${ARCH} \
  -DLLVM_ENABLE_LIBCXX=1 \
  -DEMSCRIPTEN_PRELOAD=/emsdk_portable/sdk/tools/file_packager.py \
  -DLLVM_TABLEGEN=/usr/src/patch/llvm-${LLVM_VERSION}-tblgen -DCLANG_TABLEGEN=/usr/src/patch/clang-${LLVM_VERSION}-tblgen \
  -GNinja \
	/usr/src

ninja
ninja test
ninja install
