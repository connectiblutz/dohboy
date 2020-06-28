#!/usr/bin/env bash

set -eu

service dns2doh stop || true

rm -f /etc/systemd/system/dns2doh.service
