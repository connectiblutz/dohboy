#!/usr/bin/env bash

set -eu

service dohboy stop || true

systemctl disable dohboy