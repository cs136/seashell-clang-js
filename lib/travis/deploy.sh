#!/bin/bash -e
# Copy the install directory
cp -R build/install seashell-clang-js
# Archive it
tar czf seashell-clang-js.tar.xz seashell-clang-js
BINTRAY_FILE="seashell-clang-js.tar.xz"

deploy () {
  BINTRAY_PACKAGE="$1"
  BINTRAY_VERSION="$2"
  BINTRAY_RESPONSE=`curl -T "${BINTRAY_FILE}" "-ucs136:${BINTRAY_KEY}" "https://api.bintray.com/content/cs136/seashell-clang-js/$1/$2/${BINTRAY_FILE}?publish=1&override=1"`

  if [ '{"message":"success"}' == "${BINTRAY_RESPONSE}" ]; then
    echo "Artifact published at Bintray!"
  else
    echo "Depolyment to Bintray failed with response ${BINTRAY_RESPONSE}"
    exit 1
  fi
}
if [ "${TRAVIS_PULL_REQUEST}" -eq "false" ]; then
  deploy "binaries" "B${TRAVIS_BUILD_NUMBER}"
  deploy "binaries" "${TRAVIS_BRANCH}"
  if ! [ -z "${TRAVIS_TAG}" ]; then
    deploy "binaries" "${TRAVIS_TAG}"
  fi
fi

# Deploy
#BINTRAY_VERSION="${TRAVIS_BUILD_NUMBER}"
#BINTRAY_VERSION="latest"
#BINTRAY_DIRECTORY="emscripten-sdk-test/${TRAVIS_BRANCH}"
#BINTRAY_FILE="hello-world.js"

#BINTRAY_RESPONSE=`curl -T "build/hello-world.js" "-u${BINTRAY_AUTHORIZATION}" "https://api.bintray.com/content/ooxi/generic/emscripten-sdk-test/${BINTRAY_VERSION}/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}?publish=1&override=1"`
