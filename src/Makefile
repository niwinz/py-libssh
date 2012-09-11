all: pyssh

CC=clang++ -std=c++11 -fPIC -I /usr/include/python3.2mu/
LDFLAGS=-l ssh

PYTHON_LDFLAGS=-l boost_python3 -shared

objects:
	$(CC) -c -o channel.o channel.cpp
	$(CC) -c -o session.o session.cpp
	$(CC) -c -o result.o result.cpp
	$(CC) -c -o pyssh.o pyssh.cpp

pyssh: objects
	$(CC) $(LDFLAGS) $(PYTHON_LDFLAGS) -o _pyssh.so session.o result.o channel.o pyssh.o

clean:
	rm *.o
	rm _pyssh.so
