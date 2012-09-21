from __future__ import print_function

import _pyssh
import sys


class Result(object):
    """
    Python wrapper of ExecResult
    with iteration support.
    """
    _in_iteration = False
    _cached_result = b""

    def __init__(self, _result):
        self._result = _result

    def __iter__(self):
        return self

    def __next__(self):
        self._in_iteration = True
        data = self._result.next()
        if data:
            self._cached_result += data
            return data

        self._in_iteration = False
        raise StopIteration

    def as_bytes(self):
        if self._in_iteration and not self._finished:
            raise RuntimeError("Current result is used by iterator.")

        return b"".join(list(self))

    def as_str(self, encoding="utf-8"):
        return self.as_bytes().decode(encoding)

    @property
    def return_code(self):
        return self._result.return_code

    @property
    def is_finished(self):
        return self._result.is_finished

    def next(self):
        return self.__next__()


class SftpFile(object):
    def __init__(self, _sftp_file):
        self._sftp_file = _sftp_file

    def write(self, data):
        assert isinstance(data, bytes), "data must be a bytes instance"
        return self._sftp_file.write(data)

    def seek(self, pos):
        return self._sftp_file.seek(pos)

    def tell(self):
        return self._sftp_file.tell();

    def close(self):
        return self._sftp_file.close()

    def read(self, num=-1):
        return self._sftp_file.read(num)


class SftpSession(object):
    def __init__(self, _sftp_session):
        self._sftp_session = _sftp_session

    def open(self, path, mode="w+"):
        return SftpFile(self._sftp_session.open(path, mode))

    def put(self, local_path, remote_path):
        return self._sftp_session.put(local_path, remote_path)


class Session(object):
    """
    Python wrapper for C++ Session.
    """

    def __init__(self, _session):
        self._session = _session

    def execute(self, command):
        return Result(_pyssh.execute(command, self._session))

    def disconnect(self):
        self._session.disconnect()

    def sftp_session(self):
        return SftpSession(_pyssh.SftpSession(self._session))


def connect(hostname="localhost", port=22, username=None, password=None):
    _session = _pyssh.create_session(hostname, port)
    if username is not None and password is not None:
        _session.auth(username, password)
    _session.connect()
    return Session(_session)
