#!/bin/bash

set -eu

BUILD_VERSION=0.2.0
DIST_PATH=""
NATIVE_OUT_PATH=""
PLATFORM="linux"
ARCH="arm"
DEB_ARCH="armhf"
BIN_PATH="/usr/share/dns2doh"
MAINTAINER="Brian Lutz <>"
DESCRIPTION="DNS to DoH proxy"
VENDOR="Brian Lutz"
HOMEPAGE="https://github.com/connectiblutz/dns2doh"


while getopts a:p: option
do
case "${option}"
in
a) ARCH=${OPTARG};;
p) PLATFORM=${OPTARG};;
esac
done



if [ "${ARCH}" = "arm" ]; then
  DEB_ARCH="armhf"
elif [ "${ARCH}" = "arm64" ]; then
  DEB_ARCH="arm64"
elif [ "${ARCH}" = "x64" ]; then
  DEB_ARCH="amd64"
fi

NATIVE_OUT_PATH="../_builds/${PLATFORM}/${ARCH}/Release/dns2doh"

DIST_PATH="../_dist/${PLATFORM}/${ARCH}"

echo "BUILD_VERSION = ${BUILD_VERSION}"
echo "NATIVE_OUT_PATH = ${NATIVE_OUT_PATH}"
echo "DIST_PATH = ${DIST_PATH}"
echo "PLATFORM = ${PLATFORM}"
echo "ARCH = ${ARCH}"

SYSTEMD_FILE="./service/dns2doh.service"

declare -a CommonFiles=("./service/dns2doh.conf")

rm -rf "$DIST_PATH"
mkdir -p "${DIST_PATH}${BIN_PATH}"

for val in "${CommonFiles[@]}"; do
  cp -v "$val" "${DIST_PATH}${BIN_PATH}/"
done

cp -v "${NATIVE_OUT_PATH}/dns2doh" "${DIST_PATH}${BIN_PATH}/dns2doh"

wd=$PWD

cd "${DIST_PATH}" || exit

fpm -s dir -t deb -v "${BUILD_VERSION}" -a "${DEB_ARCH}" \
  -n dns2doh -m "${MAINTAINER}" \
  --description "${DESCRIPTION}" \
  --vendor "${VENDOR}" \
  --url "${HOMEPAGE}" \
  --before-remove ../../../packaging/preRm.sh \
  --after-remove ../../../packaging/postRm.sh \
  --before-install ../../../packaging/preInst.sh \
  --after-install ../../../packaging/postInst.sh \
  --before-upgrade ../../../packaging/preInst.sh \
  --after-upgrade ../../../packaging/postInst.sh \
  --deb-systemd ../../../packaging/${SYSTEMD_FILE} \
  .

mkdir -p ../../../_out/
rm -f ../../../_out/*_${DEB_ARCH}.deb
mv *.deb ../../../_out/

cd - || exit
