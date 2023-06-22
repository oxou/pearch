#!/bin/bash

if [ $EUID != 0 ]; then
    echo "You must be root to use this script."
    exit 1
fi

echo "#!/usr/bin/env php" >> /usr/bin/pearch
cat cli.php >> /usr/bin/pearch
chmod +x /usr/bin/pearch

echo "pearch installed."
