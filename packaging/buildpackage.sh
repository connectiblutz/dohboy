#!/bin/bash

set -eu

BUILD_VERSION=0.2.0
DIST_PATH=""
NATIVE_OUT_PATH=""
PLATFORM="linux"
ARCH="arm"
DEB_ARCH="armhf"
PKG_NAME_ARCH="armhf"
BIN_PATH="/usr/share/dohboy"
MAINTAINER="Brian Lutz"
DESCRIPTION="DNS to DoH proxy"
VENDOR="Brian Lutz"
HOMEPAGE="https://github.com/connectiblutz/dohboy"


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
  PKG_NAME_ARCH="armhf"
elif [ "${ARCH}" = "armv6" ]; then
  DEB_ARCH="armhf"
  PKG_NAME_ARCH="armv6"
elif [ "${ARCH}" = "arm64" ]; then
  DEB_ARCH="arm64"
  PKG_NAME_ARCH="arm64"
elif [ "${ARCH}" = "x64" ]; then
  DEB_ARCH="amd64"
  PKG_NAME_ARCH="amd64"
fi

NATIVE_OUT_PATH="../_builds/${PLATFORM}/${ARCH}/Release/dohboy"

DIST_PATH="../_dist/${PLATFORM}/${ARCH}"

echo "BUILD_VERSION = ${BUILD_VERSION}"
echo "NATIVE_OUT_PATH = ${NATIVE_OUT_PATH}"
echo "DIST_PATH = ${DIST_PATH}"
echo "PLATFORM = ${PLATFORM}"
echo "ARCH = ${ARCH}"

SYSTEMD_FILE="./service/dohboy.service"

declare -a CommonFiles=("./service/dohboy.conf"
                        "./service/cacert.pem")

rm -rf "$DIST_PATH"
mkdir -p "${DIST_PATH}${BIN_PATH}"

for val in "${CommonFiles[@]}"; do
  cp -v "$val" "${DIST_PATH}${BIN_PATH}/"
done

cp -v "${NATIVE_OUT_PATH}/dohboy" "${DIST_PATH}${BIN_PATH}/dohboy"

wd=$PWD

cd "${DIST_PATH}" || exit

fpm -s dir -t deb -v "${BUILD_VERSION}" -a "${DEB_ARCH}" \
  -n dohboy -m "${MAINTAINER}" \
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
  -d libcurl4 -d openssl \
  .

mkdir -p ../../../_out/
rm -f ../../../_out/*_${PKG_NAME_ARCH}.deb
mv *.deb ../../../_out/dohboy_${BUILD_VERSION}_${PKG_NAME_ARCH}.deb

cd - || exit
