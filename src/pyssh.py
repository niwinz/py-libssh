from __future__ import print_function, unicode_literals

import _pyssh
import sys

class UnicodeMixin(object):
    if sys.version_info > (3, 0):
        __str__ = lambda x: x.__unicode__()
    else:
        __str__ = lambda x: unicode(x).encode('utf-8')


class Result(UnicodeMixin, object):
    """
    Python wrapper of ExecResult
    with iteration support.
    """

    def __init__(self, _result):
        self._result = _result

    def __iter__(self):
        return self

    def __next__(self):
        data = self._result.next()
        if data:
            return data

        raise StopIteration

    def __unicode__(self):
        return "".join(list(self))

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
        return Result(self._session.execute(command))

    def disconnect(self):
        self._session.disconnect()

    def sftp_session(self):
        return SftpSession(_pyssh.SftpSession(self._session))



def connect(hostname="localhost", port=22):
    _session = _pyssh.connect(hostname, port)
    return Session(_session)
