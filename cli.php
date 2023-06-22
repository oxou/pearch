<?php

//
// Copyright (C) 2023 Nurudin Imsirovic <github.com/oxou>
//
// pearch - Get CPU architecture from PE file
//
// Created: 2023-06-22 06:08 AM
// Updated: 2023-06-22 10:33 AM
//

// Source: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#machine-types
$machine_type = [
    0x0    => "unknown",
    0x184  => "alpha",
    0x248  => "alpha64",
    0x1D3  => "am33",
    0x8664 => "amd64",
    0x1C0  => "arm little-endian",
    0xAA64 => "arm64 little-endian",
    0x1C4  => "armnt little-endian",
    0x284  => "axp64 (same as alpha64)",
    0xEBC  => "efi byte code",
    0x14C  => "i386",
    0x200  => "ia64",
    0x6232 => "loongarch32",
    0x6264 => "loongarch64",
    0x9041 => "m32r",
    0x266  => "mips16",
    0x366  => "mipsfpu",
    0x466  => "mipsfpu16",
    0x1F0  => "powerpc",
    0x1F1  => "powerpcfp",
    0x166  => "r4000 (mips little-endian)",
    0x5032 => "riscv32",
    0x5064 => "riscv64",
    0x5128 => "riscv128",
    0x1A2  => "sh3 (hitachi)",
    0x1A3  => "sh3dsp (hitachi)",
    0x1A6  => "sh4 (hitachi)",
    0x1A8  => "sh5 (hitachi)",
    0x1C2  => "thumb",
    0x169  => "wcemipsv2 (mips little-endian)"
];

function cli_usage() {
    echo <<<A
Usage: pearch <path(s) to each executable>
A;
}

function cli_file($path, $msg) {
    echo "$path: $msg\n";
}

$files = $argv;
array_shift($files);
$files_len = sizeof($files);

if (1 > $files_len) {
    cli_usage();
    exit(1);
}

foreach ($files as $file) {
    $path = realpath($file);

    if (is_dir($file)) {
        cli_file($path, "Not a file");
        continue;
    }

    $filesize = filesize($file);

    if ($filesize == 0) {
        cli_file($path, "Empty");
        continue;
    }

    // -------------------------------------------------

    $handle = fopen($file, 'r');
    $offset = 0;

    fseek($handle, $offset, SEEK_SET);

    // If the file header does not match that of
    // a Windows executable we immediately bail.
    if (fread($handle, 2) !== "MZ") {
        fclose($handle);
        cli_file($path, "Not an executable");
        continue;
    }

    // If the file is smaller than the offset defining
    // the address to the COFF header, this means that
    // the executable file is malformed or incorrect.
    if (60 > $filesize) {
        fclose($handle);
        cli_file($path, "Malformed file, smaller than DOS header");
        continue;
    }

    fseek($handle, 0x3C, SEEK_SET);

    $coff_offset = bin2hex(fread($handle, 4));
    $coff_offset = str_split($coff_offset, 2);
    $coff_offset = array_reverse($coff_offset);
    $coff_offset = implode('', $coff_offset);
    $coff_offset = hexdec($coff_offset);

    // If the COFF header offset is bigger than the
    // size of the file itself, we bail as this is
    // a violation.
    if ($coff_offset > $filesize) {
        fclose($handle);
        cli_file($path, "Malformed file, COFF header offset > file size");
        continue;
    }

    fseek($handle, $coff_offset + 4, SEEK_SET);

    $arch = bin2hex(fread($handle, 2));
    $arch = str_split($arch, 2);
    $arch = array_reverse($arch);
    $arch = implode('', $arch);
    $arch = hexdec($arch);

    if (!array_key_exists($arch, $machine_type))
        $arch = 0; // unknown

    fclose($handle);
    cli_file($path, "Architecture: " . $machine_type[$arch]);
    continue;
}

?>
