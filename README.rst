=========
py-libssh
=========

Python wrapper for libssh build on top of boost::python.
With full streaming api.

Dependences:

* boost >= 1.50
* python >=3.2, 2.7
* libssh >= 0.5
* cmake >= 2.8 (build only)

**NOTE**: python 2 is not tested.

How to build
------------

On local repo root directory::

    mkdir build;
    cd build
    cmake ../src
    make

    # Copy a python module to local directory
    cp ../src/pyssh.py .
