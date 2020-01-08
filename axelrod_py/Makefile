
CC = gcc 
CFLAGS = -fPIC -O3 -g -Wall
LDFLAGS = -lm

SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

.PHONY: all clean
all: libc.so


libc.so: $(OBJ)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rfv $(SRCDIR)/*.o *.so *.pyc $(SRCDIR)/*~ *~
