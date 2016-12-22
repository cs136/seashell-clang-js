#!/bin/bash -e


# Install docker
if ! dpkg -l docker.io; then
  sudo apt-get update
  sudo apt-get install -y docker.io
fi

# Specify environment
SOURCE_DIRECTORY=$PWD
TRAVIS_DIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CCACHE_DIRECTORY=$HOME/.ccache

if ! [ -d ${CCACHE_DIRECTORY} ]; then
  echo "Creating ccache directory..."
  mkdir ${CCACHE_DIRECTORY}
fi

echo "Will use \`${SOURCE_DIRECTORY}' as source, \`${CCACHE_DIRECTORY}' as cache directory, and \`${TRAVIS_DIRECTORY}' as travis directory"

# Compile to JavaScript
docker pull dockcross/browser-asmjs
docker run --rm -it -v $SOURCE_DIRECTORY:/usr/src:rw -v $TRAVIS_DIRECTORY:/travis:ro -v $CCACHE_DIRECTORY:/root/.ccache:rw dockcross/browser-asmjs /travis/compile-inside-docker.sh

# Test
#sudo apt-get install -y nodejs
#nodejs build/hello-world.js

# Deploy
#BINTRAY_VERSION="${TRAVIS_BUILD_NUMBER}"
#BINTRAY_VERSION="latest"
#BINTRAY_DIRECTORY="emscripten-sdk-test/${TRAVIS_BRANCH}"
#BINTRAY_FILE="hello-world.js"

#BINTRAY_RESPONSE=`curl -T "build/hello-world.js" "-u${BINTRAY_AUTHORIZATION}" "https://api.bintray.com/content/ooxi/generic/emscripten-sdk-test/${BINTRAY_VERSION}/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}?publish=1&override=1"`

#if [ '{"message":"success"}' == "${BINTRAY_RESPONSE}" ]; then
#	echo "Artifact published at https://dl.bintray.com/ooxi/generic/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}"
#else
#	echo "Depolyment to Bintray failed with response ${BINTRAY_RESPONSE}"
#	exit 1
#fi

