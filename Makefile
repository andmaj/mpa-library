all: static general examples

general: general-shared general-lib general-obj general-test

static: static-shared static-lib static-obj static-test

test: general-test static-test

examples: example01 example02

general-shared: bin/libmpa.so

general-lib: bin/libmpa.a

general-obj: bin/mpa.o

general-test: bin/mpa-test

static-shared: bin/libmpa-static.so

static-lib: bin/libmpa-static.a

static-obj: bin/mpa-static.o

static-test: bin/mpa-static-test8 bin/mpa-static-test16 bin/mpa-static-test32 bin/mpa-static-test15

bin/libmpa.so: src/mpa.c src/mpa.h bin/tmp
	$(CC) $(CFLAGS) -Wall -c -fPIC -o bin/tmp/mpa-shared.o src/mpa.c 
	$(CC) $(CFLAGS) -Wall -shared -Wl,-soname,libmpa.so.1 -o bin/libmpa.so bin/tmp/mpa-shared.o

bin/libmpa.a: src/mpa.c src/mpa.h bin/tmp
	$(CC) $(CFLAGS) -Wall -c -o bin/tmp/mpa-lib.o src/mpa.c
	ar rcs bin/libmpa.a bin/tmp/mpa-lib.o

bin/mpa.o: src/mpa.c src/mpa.h
	$(CC) $(CFLAGS) -Wall -c -o bin/mpa.o src/mpa.c

bin/mpa-test: src/mpa-test.c src/mpa.c src/mpa.h
	$(CC) $(CFLAGS) -Wall -o bin/mpa-test src/mpa-test.c src/mpa.c

bin/libmpa-static.so: src/mpa-static.c src/mpa-static.h bin/tmp
	$(CC) $(CFLAGS) -Wall -c -fPIC -o bin/tmp/mpa-static-shared.o src/mpa-static.c 
	$(CC) $(CFLAGS) -Wall -shared -Wl,-soname,libmpa-static.so.1 -o bin/libmpa-static.so bin/tmp/mpa-static-shared.o

bin/libmpa-static.a: src/mpa-static.c src/mpa-static.h bin/tmp
	$(CC) $(CFLAGS) -Wall -c -o bin/tmp/mpa-static-lib.o src/mpa-static.c
	ar rcs bin/libmpa-static.a bin/tmp/mpa-static-lib.o

bin/mpa-static.o: src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -c -o bin/mpa-static.o src/mpa-static.c

bin/mpa-static-test8: src/mpa-static-test.c src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -DLOG_BITSIZE=8 -o bin/mpa-static-test8 src/mpa-static-test.c src/mpa-static.c

bin/mpa-static-test16: src/mpa-static-test.c src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -DLOG_BITSIZE=16 -o bin/mpa-static-test16 src/mpa-static-test.c src/mpa-static.c

bin/mpa-static-test32: src/mpa-static-test.c src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -DLOG_BITSIZE=32 -o bin/mpa-static-test32 src/mpa-static-test.c src/mpa-static.c

bin/mpa-static-test15: src/mpa-static-test.c src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -DLOG_BITSIZE=15 -o bin/mpa-static-test15 src/mpa-static-test.c src/mpa-static.c

bin/tmp:
	mkdir bin/tmp
	
example01: examples/bin/example01
	
example02: examples/bin/example02

examples/bin/example01: examples/src/example01.c src/mpa.c src/mpa.h
	$(CC) $(CFLAGS) -Wall -I src -o examples/bin/example01 examples/src/example01.c src/mpa.c

examples/bin/example02: examples/src/example02.c src/mpa-static.c src/mpa-static.h
	$(CC) $(CFLAGS) -Wall -I src -DLOG_BITSIZE=13 -o examples/bin/example02 examples/src/example02.c src/mpa-static.c
	
clean:
	rm -rf bin/*
	rm -rf examples/bin/*
