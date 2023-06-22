
# exearch

This script provides the ability to retrieve CPU architecture from a PE file.

In order to use this script you need to have PHP installed.

# Installation

## Linux

1. `git clone --depth=1 https://github.com/oxou/exearch`
2. `cd exearch`
3. `chmod +x ./install.sh`
4. `./install.sh`

## Windows

1. `git clone --depth=1 https://github.com/oxou/exearch`
2. `cd exearch`
3. `notepad exearch.bat`
4. Set {PHP_EXE} to the absolute path of `php.exe` and {CLI_PHP} to the absolute path of `cli.php` example: `C:\exearch\cli.php`
5. Save `exearch.bat` to `C:\Windows\System32`

# Usage

## Basics

```
$ exearch
Usage: Usage: exearch <path(s) to each executable>

$ exearch /tmp/notepad_32.exe
/tmp/notepad_32.exe: Architecture: i386

$ exearch /tmp/notepad_64.exe
/tmp/notepad_64.exe: Architecture: amd64
```

## Multiple files

```
$ exearch /tmp/notepad_32.exe /tmp/notepad_64.exe /tmp/mspaint_32.exe

/tmp/notepad_32.exe: Architecture: i386
/tmp/notepad_64.exe: Architecture: amd64
/tmp/mspaint_32.exe: Architecture: i386
```
