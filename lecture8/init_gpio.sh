#!/bin/bash

cd /sys/class/gpio

echo 69 > export
echo out > direction
echo 1 > value
