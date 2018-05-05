CFLAGS = -g -O0  #Debug symbols, and no optimization

#-------------------------------------------------------------
# Makefile Targets
#-------------------------------------------------------------

HEADERS = scramble.h
OBJECTS = scramble.o

all: scramble

%.o: %.c $(HEADERS)
	gcc -c $(CFLAGS) $< -o $@

hangman: $(OBJECTS)
	gcc $(OBJECTS) $(CFLAGS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f scramble

