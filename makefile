LFLAGS = -Wall -o
CFLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors -c

all: shell

shell: shell.o
	g++ $(LFLAGS) shell shell.o
	
shell.o: shell.cpp
	g++ $(CFLAGS) shell.cpp
	
clean:
	mv *.o /home/ugrads/sutton/bin
	mv ./shell /home/ugrads/sutton/bin