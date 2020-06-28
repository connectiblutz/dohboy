#!/usr/bin/env bash

set -eu

service dns2doh stop || true

systemctl disable dns2doh