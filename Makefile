# flags and incs
CFLAGS = -Wall -g
INCS = login.c util.c

PREFIX = /usr/local 

# compiler and linker
CC = cc

all: login
login: login.o
	$(CC) login.o $(CFLAGS) -o $@
login.o: $(INCS)

clean:
	rm -f login *.o

install: login
	mkdir -p $(PREFIX)/bin
	cp -f login $(PREFIX)/bin
	chmod 755 $(PREFIX)/bin/login
uninstall: login
	rm -f $(PREFIX)/bin/login
