#!/bin/bash
SCRIPT=$(readlink -f $0)
# Absolute path this script is in, thus /home/user/bin
BASEDIR=$(dirname $SCRIPT)

cd $BASEDIR

export NETBOOK=1
export LD_LIBRARY_PATH=/usr/local/lib
exec ./Towerdefense $1
