CC	 	= gcc
BIN	 	= bin/
SRC	 	= src/
INCLUDES 	= include/
EXEC	 	= k_means
THREADS	 	= 2

CP_CLUSTERS	 = 4

CFLAGS = -std=c99 -O3 -W -Wall -Wextra -Wuninitialized -Wstrict-aliasing -ftree-vectorize -msse4 -fopt-info



.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c $(BIN)utils.o 
	$(CC) $(CFLAGS) $(SRC)k_means.c $(BIN)utils.o -o $(BIN)$(EXEC)

$(BIN)utils.o: $(SRC)utils.c $(INCLUDES)utils.h
	$(CC) $(CFLAGS) -c $(SRC)utils.c -o $(BIN)utils.o

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) 1

runpar:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) $(THREADS)
