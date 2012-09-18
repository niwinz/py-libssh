=========
py-libssh
=========

Python wrapper for libssh build on top of boost::python.
With full streaming api.

Dependences:

* boost >= 1.50
* python >=3.2
* libssh >= 0.5
* cmake >= 2.8 (build only)

And C++11 ready compiller. Tested with GCC == 4.7.1 and Clang++ == 3.1.

**NOTE**: python 2 is not tested.


How to build
------------

For development, go to repo root directory and execute these commands::

    mkdir build;
    cd build
    cmake ../src
    make

    # Copy a python module to local directory
    cp ../src/pyssh.py .


For normal use, you can use a standard python distutils ``setup.py`` file::

    python setup.py install


Api reference:
--------------


`pyssh.connect(hostname="localhost", port=22, username=None, password=None)`
    Creates ssh session and connects to corresponding host and port. By default intent autenticate with local pubkey.
    If passwrod and username are provided, normal user and password authentication is executed instead of a pubkey.

    This returns ``pyssh.Session`` instance.


``pyssh.Session``
^^^^^^^^^^^^^^^^^

Represents a ssh connection.

``pyssh.Session.execute(command)``
    Executes some command on a remote machine. Returns ``pyssh.Result`` instance.

``pyssh.Session.sftp_session()``
    Open a sftp session on top of current ssh session. Return a ``pyssh.SftpSession`` instance.


``pyssh.Result``
^^^^^^^^^^^^^^^^

Represents a result of execution of command on ssh session. Result by default, does not download all the content, but you have to iterate over all output for command execution. (The content is obtained from the server in chucks of 1024 bytes)

``pyssh.Result.__iter__()``
    Returns itself as iterator.

``pyssh.Result.__next__()``
    Iterate over command output content in chunks of 1024 bytes.

``pyssh.Result.return_code``
    Command execution return code. Only avaliable over all iteration.

``pyssh.Result.as_str()``
    Return unicode string of all command execution output.

``pyssh.Result.as_bytes()``
    Same as that, ``as_str()`` but returns a bytes.


``pyssh.SftpSession``
^^^^^^^^^^^^^^^^^^^^^

Represents a sftp connection.

``pyssh.SftpSession.put(local_path, remote_path)``
    Transfer local file to remote file.

``pyssh.SftpSession.open(remote_path, mode="w+")``
    Open remote file (with random access support). Mode can be: ``w`` (Write + Truncate), ``r`` (Read), ``w+`` (Write+Read+Truncate) and  ``r+`` (Read+Write). Returns ``pyssh.SftpFile`` instance.


``pyssh.SftpFile``
^^^^^^^^^^^^^^^^^^

Represents a opened sftp remote file with random access support. This file only works with python3 bytes or python2 str types.

``pyssh.SftpFile.write(data)``
    Write bytestring to the opened file.

``pyssh.SftpFile.read(num=-1)``
    Read content from the opened file. if num is -1, reads all content from current position to the end of file.

``pyssh.SftpFile.seek(pos)``
    Change position on the opened file.

``pyssh.SftpFile.tell()``
    Get current position on the opened file.

``pyssh.SftpFile.close()``
    Close the current file.


Examples
--------

Sftp session example.

.. code-block:: python

    >>> import pyssh
    >>> session = pyssh.connect("localhost")
    >>> sftp_session = session.sftp_session()
    >>> f = sftp_session.open("/tmp/some-file", "w+")
    >>> f.tell()
    0
    >>> f.write(b'Hello World')
    >>> f.tell()
    11
    >>> f.seek(0)
    True
    >>> f.read(5)
    b'Hello'
    >>> f.read()
    b' World'


Command execution example.

.. code-block:: python 

    >>> import pyssh
    >>> session = pyssh.connect("localhost")
    >>> result = session.execute("uname -a")
    >>> print(result)
    Linux vaio.niwi.be 3.5.3-1-ARCH #1 SMP PREEMPT Sun Aug 26 09:14:51 CEST 2012 x86_64 GNU/Linux

    >>> result.return_code
    0

