#include <stdio.h>
#include <stdlib.h>

#define my_malloc(size) renamed_malloc(size)
#define my_free(ptr) renamed_free(ptr)

void* renamed_malloc(size_t size) {
    printf("Custom malloc called\n");
    return malloc(size);
}

void renamed_free(void* ptr) {
    printf("Custom free called\n");
    free(ptr);
}

int main() {
    void* ptr = my_malloc(10);
    my_free(ptr);
    return 0;
}
