#include <stdio.h>

extern void memory_overrun();

int main() {
    printf("Calling memory_overrun() from MemoryOverrun library...\n");
    memory_overrun();
    printf("Returned from memory_overrun()\n");
    return 0;
}