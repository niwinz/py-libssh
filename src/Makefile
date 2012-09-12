all: pyssh

CC=clang++ -std=c++11 -fPIC -I /usr/include/python3.2mu/ -O3
LDFLAGS=-l ssh

PYTHON_LDFLAGS=-l boost_python3 -shared

objects:
	$(CC) -c -o ssh.o ssh.cpp
	$(CC) -c -o pyssh.o pyssh.cpp

pyssh: objects
	$(CC) $(LDFLAGS) $(PYTHON_LDFLAGS) -o _pyssh.so ssh.o pyssh.o

clean:
	rm *.o
	rm _pyssh.so
