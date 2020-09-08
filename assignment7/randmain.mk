C = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) -o randmain randmain.o randcpuid.o

randmain.o: randmain.c randcpuid.h
	$(CC) $(CFLAGS) -c randmain.c

randcpuid.o: randcpuid.c randcpuid.h
	$(CC) $(CFLAGS) -c randcpuid.c

randlibhw.o: randlibhw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c

randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o

randlibsw.o: randlibsw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c

randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o
