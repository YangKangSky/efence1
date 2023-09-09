#include <stdio.h>
#include <stdlib.h>

void memory_overrun() {
    int* ptr = (int*)malloc(sizeof(int) * 5);
    int i;
    for (i = 0; i <= 5; i++) {  // 越界访问数组，访问了 ptr[5]
        printf("%d\n", ptr[i]);
    }
	printf("free\n");
    free(ptr);
	printf("end\n");
}