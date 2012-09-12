all: pyssh

CXX=clang++ -std=c++11 -fPIC -I /usr/include/python3.2mu/ -O3
LDFLAGS=-l ssh -l boost_python3 -l boost_filesystem -shared

# python2
#CXX=g++ -std=c++11 -fPIC -I /usr/include/python2.7/ -O3
#LDFLAGS=-l ssh -l boost_python -shared

objects:
	mkdir -p .build
	$(CXX) -c -o .build/ssh.o ssh.cpp
	$(CXX) -c -o .build/sftp.o sftp.cpp
	$(CXX) -c -o .build/pyssh.o pyssh.cpp

pyssh: objects
	$(CXX) $(LDFLAGS) -o _pyssh.so .build/ssh.o .build/sftp.o .build/pyssh.o

clean:
	rm -rf .build
	rm _pyssh.so
