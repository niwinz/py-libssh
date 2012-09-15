from __future__ import print_function, unicode_literals
import _pyssh

class Result(object):
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


def connect(hostname="localhost", port=22):
    _session = _pyssh.connect(hostname, port)
    return Session(_session)
