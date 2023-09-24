
# pearch

pearch is a CLI utility to retrieve CPU architecture from
a PE file.

# Installation

See [Releases](https://github.com/oxou/pearch/releases).

## Alternative

If you're on Linux, you can compile the executable using GCC.
Use `sudo ./make.sh && sudo ./install.sh` to compile and install
pearch.

for Windows you can use
[Tiny C Compiler](https://bellard.org/tcc/) and quickly compile
using the command `tcc main.c -o pearch.exe`.

To compile for 32-bit and 64-bit, use the `-m32` and `-m64` flag.

# Usage

## Basics

```
$ pearch
Usage: pearch <path(s) to each executable>

$ pearch /tmp/notepad_32.exe
/tmp/notepad_32.exe: Architecture: i386

$ pearch /tmp/notepad_64.exe
/tmp/notepad_64.exe: Architecture: amd64
```

## Multiple files

```
$ pearch /tmp/a_32.exe /tmp/b_64.exe /tmp/c_32.exe

/tmp/a_32.exe: Architecture: i386
/tmp/b_64.exe: Architecture: amd64
/tmp/c_32.exe: Architecture: i386
```

## Extra

Use `grep` to match specific files, for example all x86 files
in the system32 folder:

```
$ cd /c/windows/system32
$ pearch *.exe | grep "i386$" | awk -F ": Arch" '{print $1}'
Microsoft.Uev.SyncController.exe
UevAgentPolicyGenerator.exe
UevAppMonitor.exe
UevTemplateBaselineGenerator.exe
UevTemplateConfigItemGenerator.exe
stordiag.exe
```

# Missing Features

Globbing is not yet supported on Windows platforms.
Use busybox bash on Windows to have this support.
