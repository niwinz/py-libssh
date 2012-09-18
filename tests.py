import io
import os
import sys
import shutil
import unittest
import importlib


class PythonLibsshTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Add build directory to python path
        current_dir = os.path.dirname(__file__)
        sys.path.insert(0, os.path.join(current_dir, "build"))

        src_dir = os.path.join(current_dir, "src")
        shutil.copy(os.path.join(src_dir, "pyssh.py"), \
            os.path.join(current_dir, "build", "pyssh.py"))

        cls.pyssh = importlib.import_module("pyssh")

        with io.open("/tmp/py-libssh.temp.file", "wb") as f:
            f.write(b"aaaaaaaaa\n")
            f.write(b"bbbbbbbbb\n")
            f.write(b"ccccccccc\n")

    @classmethod
    def tearDownClass(cls):
        os.remove("/tmp/py-libssh.temp.file")

    def test_connect_and_execute_command_success(self):
        s = self.pyssh.connect()
        r = s.execute("uname")
        result = r.as_bytes()
        return_code = r.return_code

        s.disconnect()

        self.assertEqual(return_code, 0)
        self.assertEqual(result, b"Linux\n")
