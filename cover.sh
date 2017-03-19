#!/bin/sh
if [ $# != 1 ]
then
    echo "Usage: conv2utf-8.sh FOLDERNAME"
    echo "Eg: conv2utf-8.sh ."
fi
find $1 -type f -exec enca -x utf8 {} \;
