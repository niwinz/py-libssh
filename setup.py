# -*- coding: utf-8 -*-

from distutils.core import Extension, setup
from distutils.command.build_ext import build_ext
from distutils.command.install_lib import install_lib

from os.path import exists, join, dirname, normpath
from os import mkdir, remove, chdir, getcwd
from subprocess import Popen
import sys, shutil

class my_install_lib(install_lib):
    build_directory =  "build"
    current_path = getcwd()

    def install(self):
        shutil.copy(join(self.current_path, self.build_dir, "_pyssh.so"), join(self.install_dir, "_pyssh.so"))
        shutil.copy(join(self.current_path, self.build_dir, "pyssh.py"), join(self.install_dir, "pyssh.py"))


class my_build_ext(build_ext):
    build_directory =  "build"
    current_path = getcwd()

    def prepare_py_libssh(self):
        build_path = join(self.current_path, self.build_directory)
        src_path = join(self.current_path, "src")

        if not exists(build_path):
            mkdir(build_path)

        chdir(build_path)
        ret_code = Popen(["cmake", "-DCMAKE_BUILD_TYPE=Release", src_path]).wait()
        if ret_code != 0:
            sys.exit(ret_code)

        ret_code = Popen(["make"]).wait()
        if ret_code != 0:
            sys.exit(ret_code)

        shutil.copy(join(build_path, "_pyssh.so"), join(self.current_path,  self.build_lib))

    def build_extension(self, ext):
        return self.prepare_py_libssh()


setup(
    name='pyssh',
    version='0.1.0',
    description='Python bingings for libssh build on top of boost.python',
    author='Andrey Antukh',
    author_email='niwi@niwi.be',

    url='https://github.com/niwibe/py-libssh',
    package_dir = {'': 'src'},
    py_modules = ['pyssh'],
    cmdclass=dict(build_ext=my_build_ext, install_lib=my_install_lib),

    ext_modules=[Extension('_pyssh', [])],
)
