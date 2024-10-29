# This is a comment line
CC=g++
# This is a folder to look for header files
INCDIR=./include
DBUG=./build/debug
RELEASE=./build/release

# CFLAGS will be the options passed to the compiler.
CFLAGS= -c -I$(INCDIR) -fopenmp 
#LDFLAGS= -lm -lblas

ROBJECTS= $(RELEASE)/main.o #$(RELEASE)/matMul.o
DBUGOBJECTS= $(DBUG)/main.o #$(DBUG)/matMul.o

DBUGFLAGS=$(CFLAGS) -Wall -g -fsanitize=undefined -fsanitize=undefined -fsanitize=leak 
RFLAGS=$(CFLAGS) -O3

release:$(ROBJECTS)
	$(CC) $(ROBJECTS) -o ex_rel.x $(LDFLAGS) -fopenmp

debug:$(DBUGOBJECTS)
	$(CC) $(DBUGOBJECTS) -o ex_dgb.x $(LDFLAGS) -fsanitize=undefined -fsanitize=undefined -fsanitize=leak -fopenmp

$(RELEASE)/main.o: main.cpp
	$(CC) $(RFLAGS) main.cpp -o $@

#$(RELEASE)/matMul.o: ./src/matMul.c
#	$(CC) $(RFLAGS) ./src/matMul.c -o $@ $(LDFLAGS)

$(DBUG)/main.o: main.cpp
	$(CC) $(DBUGFLAGS) main.cpp -o $@

#$(DBUG)/matMul.o: ./src/matMul.c
#	$(CC) $(DBUGFLAGS) ./src/matMul.c -o $@ $(LDFLAGS)

clean:
	rm -rf $(DBUG)/*.o $(RELEASE)/*.o *x
