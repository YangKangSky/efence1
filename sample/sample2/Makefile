CC = gcc
CFLAGS = -Wall -Werror




LIB_NAME = libMemoryOverrun.so

LIB_MYMALLOC = libMyMalloc.so

.PHONY: $(LIB_NAME) $(LIB_MYMALLOC)

all: test

test: test.c $(LIB_NAME) $(LIB_MYMALLOC)
	$(CC) $(CFLAGS) -o test test.c -L. -lMemoryOverrun  -lpthread 

#LDFLAGS += -L. -lefence -Wl,--wrap,malloc -Wl,--wrap,realloc -Wl,--wrap,calloc -Wl,--wrap,free -lpthread 

LDFLAGS += -L. -lMyMalloc -Wl,--wrap,malloc -Wl,--wrap,realloc -Wl,--wrap,calloc -Wl,--wrap,free -lpthread 


$(LIB_NAME): MemoryOverrun.c $(LIB_MYMALLOC)
	$(CC) $(CFLAGS) -shared -fPIC -o $(LIB_NAME) MemoryOverrun.c -lpthread $(LDFLAGS)

$(LIB_MYMALLOC): MyMalloc.c
	$(CC) $(CFLAGS) -shared -fPIC -o $(LIB_MYMALLOC) MyMalloc.c -lpthread

clean:
	rm -f $(LIB_NAME) test $(LIB_MYMALLOC)



#LD_PRELOAD=/path/to/mylib.so ./myprogram
LD_PRELOAD=./libefence.so ./test