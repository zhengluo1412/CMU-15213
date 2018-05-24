CC=clang
CFLAGS=-g -Werror -std=c99

default:
	@$(MAKE) test -s

.PHONY: handin test

test: cord.c cord-test.c
	$(CC) $(CFLAGS) hdict.c cord.c cord-test.c -o cord-test
	./cord-test

handin: cord.c
	tar -czvf cordhandin.tgz cord.c

