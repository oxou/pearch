/*
 * Copyleft Nurudin Imsirovic <github.com/oxou>
 *
 * pearch - Get CPU architecture from PE file
 *
 * Created: 2023-09-22 05:43 AM
 * Updated: 2023-09-25 06:38 AM
 */

/*
 * Compile with TCC or GCC:
 *   tcc main.c -o pearch.exe
 *   gcc main.c -o pearch.exe
 */

#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>

int is_dir(const char *path) {
    struct stat st;

    if (stat(path, &st) != 0)
        return 0;

    return S_ISDIR(st.st_mode);
}

int filesize(const char *path) {
    if (is_dir(path))
        return -1;

    struct stat st;

    if (stat(path, &st) == 0)
        return st.st_size;

    return 0;
}

int process_file(const char *path) {
    if (is_dir(path)) {
        printf("%s: Is a directory\n", path);
        return 0;
    }

    FILE *file;
    file = fopen(path, "rb");

    if (file == NULL) {
        printf("%s: Cannot open file\n", path);
        return 0;
    }

    int file_size = filesize(path);

    if (file_size == 0) {
        printf("%s: File is empty\n", path);
        fclose(file);
        return 0;
    }

    char magic[2];

    if (file_size > 2) {
        fread(magic, sizeof(magic), 1, file);
        magic[2] = '\0';

        if (strcmp(magic, "MZ") != 0) {
            printf("%s: Not an executable\n", path);
            fclose(file);
            return 0;
        }
    }

    if (0x3C > file_size) {
        printf("%s: Malformed file, smaller than DOS header\n", path);
        fclose(file);
        return 0;
    }

    fseek(file, 0x3C, SEEK_SET);

    uint32_t coff_offset;
    fread(&coff_offset, sizeof(coff_offset), 1, file);

    if (coff_offset > file_size) {
        printf("%s: Malformed file, COFF header offset > file size\n", path);
        fclose(file);
        return 0;
    }

    fseek(file, coff_offset + 4, SEEK_SET);

    // uint16_t = 2 bytes = 16-bits
    uint16_t machine_type_int;
    fread(&machine_type_int, sizeof(machine_type_int), 1, file);
    // We don't need to keep the file open anymore
    fclose(file);

    char *machine_type;

    // Source: https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#machine-types
    switch (machine_type_int) {
        default:
        case 0x0:    machine_type = "unknown"; break;
        case 0x184:  machine_type = "alpha"; break;
        case 0x248:  machine_type = "alpha64"; break;
        case 0x1D3:  machine_type = "am33"; break;
        case 0x8664: machine_type = "amd64"; break;
        case 0x1C0:  machine_type = "arm little-endian"; break;
        case 0xAA64: machine_type = "arm64 little-endian"; break;
        case 0x1C4:  machine_type = "armnt little-endian"; break;
        case 0x284:  machine_type = "axp64 (same as alpha64)"; break;
        case 0xEBC:  machine_type = "efi byte code"; break;
        case 0x14C:  machine_type = "i386"; break;
        case 0x200:  machine_type = "ia64"; break;
        case 0x6232: machine_type = "loongarch32"; break;
        case 0x6264: machine_type = "loongarch64"; break;
        case 0x9041: machine_type = "m32r"; break;
        case 0x266:  machine_type = "mips16"; break;
        case 0x366:  machine_type = "mipsfpu"; break;
        case 0x466:  machine_type = "mipsfpu16"; break;
        case 0x1F0:  machine_type = "powerpc"; break;
        case 0x1F1:  machine_type = "powerpcfp"; break;
        case 0x166:  machine_type = "r4000 (mips little-endian)"; break;
        case 0x5032: machine_type = "riscv32"; break;
        case 0x5064: machine_type = "riscv64"; break;
        case 0x5128: machine_type = "riscv128"; break;
        case 0x1A2:  machine_type = "sh3 (hitachi)"; break;
        case 0x1A3:  machine_type = "sh3dps (hitachi)"; break;
        case 0x1A6:  machine_type = "sh4 (hitachi)"; break;
        case 0x1A8:  machine_type = "sh5 (hitachi)"; break;
        case 0x1C2:  machine_type = "thumb"; break;
        case 0x169:  machine_type = "wcemipsv2 (mips little-endian)"; break;
    }

    printf("%s: Architecture: %s\n", path, machine_type);

    return 0;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: pearch <path(s) to each executable>\n");
        return 1;
    }

    for (unsigned int i = 1; i < argc; i++)
        process_file(argv[i]);

    return 0;
}
