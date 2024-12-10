#!/bin/bash

# print the maxium value a PID can have
pid_max=$(cat /proc/sys/kernel/pid_max)
echo "This is MAX value of PID ID: $pid_max"

