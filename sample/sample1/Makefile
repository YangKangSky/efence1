PIC= -fPIC
CFLAGS= -g -DUSE_SEMAPHORE $(PIC)
LIBS= -lpthread

prefix=/usr
BIN_INSTALL_DIR= $(prefix)/bin
LIB_INSTALL_DIR= $(prefix)/lib
MAN_INSTALL_DIR= $(prefix)/man/man3

CC= cc
AR= ar
INSTALL= install


all: eftest_static eftest_share test.o

eftest_static:  test.o
	- rm -f eftest
	$(CC) $(CFLAGS) test.o ../../libefence.a -o eftest_static $(LIBS)

eftest_share:  test.o
	- rm -f eftest
	$(CC) $(CFLAGS) -o eftest_share test.c -L./../../ -lefence  -lpthread 

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


clean:
	rm -rf eftest_static eftest_share