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

echo "Will use \`${SOURCE_DIRECTORY}' as source, \`${CCACHE_DIRECTORY}' as cache directory, and \`${TRAVIS_DIRECTORY}' as Docker directory"

# Compile to JavaScript
docker rmi seashell-builder || true
docker build -t seashell-builder - < ${TRAVIS_DIRECTORY}/../Dockerfile.in
docker run --rm -it -v $SOURCE_DIRECTORY:/usr/src:rw -v $TRAVIS_DIRECTORY:/travis:ro -v $CCACHE_DIRECTORY:/root/.ccache:rw seashell-builder /travis/compile-inside-docker.sh
