#!/bin/bash

if [ $EUID != 0 ]; then
    echo "You must be root to use this script."
    exit 1
fi

echo "#!/usr/bin/env php" >> /usr/bin/exearch
cat cli.php >> /usr/bin/exearch
chmod +x /usr/bin/exearch

echo "exearch installed."
