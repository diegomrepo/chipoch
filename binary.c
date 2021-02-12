#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    unsigned char buffer[4096] = {0};
    FILE *pFile;
    long lSize;
    size_t result;

    const char *rom_path = "roms3/fishie.ch8";

    pFile = fopen(rom_path, "rb");
    if (pFile == NULL) {
        fputs("File error", stderr);
        exit(1);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        fputs("Reading error", stderr);
        exit(3);
    }

    /* the whole file is now loaded in the memory buffer. */
    printf("Size of buffer: %ld bytes \n", sizeof(buffer));
    for (int i = 0; i < lSize; i = i + 2)
        printf("%02x%02x  ", buffer[i], buffer[i + 1]);
    putchar('\n');
    // terminate
    fclose(pFile);
    // free(buffer);

    return 0;
}
