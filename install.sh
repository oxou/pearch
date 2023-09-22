#!/bin/bash

if [ $EUID != 0 ]; then
    echo "You must be root to use this script."
    exit 1
fi

chmod +x pearch
cp pearch /usr/bin/pearch

echo "pearch installed."
