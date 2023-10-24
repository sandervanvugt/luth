#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <amount_of_memory_in_MB>\n", argv[0]);
        return 1;
    }

    size_t size = strtoul(argv[1], NULL, 10) * 1024 * 1024; // Convert from MB to Bytes

    char *mem = malloc(size);

    if (!mem) {
        perror("malloc");
        return 1;
    }

    // Actively use the allocated memory to ensure it's resident
    memset(mem, 0, size);

    printf("Allocated and used %zu bytes of resident memory. Press any key to exit...\n", size);
    getchar();

    free(mem);
    return 0;
}

