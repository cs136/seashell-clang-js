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
