CC=g++
CFLAGS= -Wall -ansi -pedantic   -c -std=c++11   -g3 -O0 
LDFLAGS= -pthread -lpthread  -lmyolinux -L /usr/local/lib/ /usr/lib/libta_lib.a -lusb-1.0 -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-network -lta_lib
EXEC=yarm
SRCDIR=src
ODIR=obj
INCLUDEDIR=include
SRC= $(wildcard $(SRCDIR)/*.cpp)
OBJ= $(subst $(SRCDIR),$(ODIR),$(SRC:.cpp=.o))


all: $(EXEC)

yarm: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


$(ODIR)/%.o: $(SRCDIR)/%.cpp
	$(CC)  -I $(INCLUDEDIR) -cpp -o $@ $< $(CFLAGS)
.PHONY: clean mrproper

clean:
	rm -rf $(ODIR)/*.o

mrproper: clean
	rm -rf $(EXEC)

rebuild: mrproper yarm
