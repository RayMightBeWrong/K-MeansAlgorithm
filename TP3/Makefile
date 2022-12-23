CC	 	= nvcc
BIN	 	= bin/
SRC	 	= src/
INCLUDES 	= include/
EXEC	 	= k_means
CP_CLUSTERS	= 4
THREADS	 	= 32

CFLAGS = -O3 -g -std=c++11 -arch=sm_35 -Wno-deprecated-gpu-targets


.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.cu $(BIN)utils.o 
	$(CC) $(CFLAGS) $(SRC)k_means.cu $(BIN)utils.o -o $(BIN)$(EXEC)

$(BIN)utils.o: $(SRC)utils.cu $(INCLUDES)utils.h
	$(CC) $(CFLAGS) -c $(SRC)utils.cu -o $(BIN)utils.o

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS)

runpar:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) $(THREADS)
