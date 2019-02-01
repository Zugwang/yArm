CC=g++
CFLAGS= -Wall -ansi -pedantic   -c -std=c++11 -pthread -lpthread  -g3 -O0 -lmyolinux -L /usr/local/lib/ -lusb-1.0
LDFLAGS= -Wall -ansi -pedantic  -std=c++11 -pthread -lpthread -g3 -O0 -lmyolinux -L /usr/local/lib/ -lusb-1.0
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
