from __future__ import print_function, unicode_literals
import sys; sys.path.insert(0, "../src")

import pyssh

session = pyssh.connect("localhost", 22)
result = session.execute("echo 'Hólä'")
print("Result:")
for item in result:
    print( (type(item), item))
print("Code:", result.return_code)
