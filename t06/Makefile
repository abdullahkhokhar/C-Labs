SHELL = /bin/bash

all: basic_test

.PHONY: clean
clean:
	rm serialize deserialize *.o

%.o: %.c helpers.h
	gcc -c $< -o $@

serialize: serialize.o helpers.o
	gcc serialize.o helpers.o -o serialize

deserialize: deserialize.o helpers.o
	gcc deserialize.o helpers.o -o deserialize

basic_test: serialize deserialize
	@output=`./serialize < test.dat`; \
	if [ "$$?" -eq 0 ]; then \
		`./deserialize <<< $$output`; \
		if [ "$$?" -eq 0 ]; then \
			echo Basic test passed; \
		else \
			echo Basic test failed; \
		fi; \
	else \
		echo Basic test failed; \
	fi;
