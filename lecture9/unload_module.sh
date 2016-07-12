#!/bin/bash

module="saw"
device="saw"

/sbin/rmmod ./$module.ko $*

rm -f /dev/${device}
