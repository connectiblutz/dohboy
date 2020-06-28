#!/usr/bin/env bash

set -eu

systemctl enable speedify

service dns2doh start