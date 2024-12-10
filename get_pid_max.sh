#!/bin/bash

if  [-f /proc/sys/kernel/pid_max]; then
    pid_max=$(cat /proc/sys/kernel/pid_max)
    echo "Maximum process ID Value: $pid_max"
else
    echo "Error: Unable to read /proc/sys/kernel/pid_max"
    echo "This script may need to be run on a Linux System."
    exit 1
fi
