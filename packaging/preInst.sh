#!/usr/bin/env bash

set -eu

service dohboy stop || true

rm -f /etc/systemd/system/dohboy.service
