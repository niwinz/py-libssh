import sys; sys.path.insert(0, "../src")

import pyssh

session = pyssh.connect("localhost", 22)
result = session.execute("uname -a")
print("Result:")
for item in result:
    print(item)
print("Code:", result.return_code)
