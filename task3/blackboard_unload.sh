#!/bin/bash

module="blackboard"
device="blackboard"

/sbin/rmmod ./$module.ko $*

rm -f /dev/${device}
