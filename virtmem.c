#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <amount_of_memory_in_MB>\n", argv[0]);
        return 1;
    }

    size_t size = strtoul(argv[1], NULL, 10) * 1024 * 1024; // Convert from MB to Bytes

    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Allocated %zu bytes of virtual memory at %p. Press any key to exit...\n", size, mem);
    getchar();

    munmap(mem, size);
    return 0;
}

