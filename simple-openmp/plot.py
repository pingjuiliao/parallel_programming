#!/usr/bin/python
import os

filepath = "./output"
if os.path.exists(filepath) :
    os.unlink(filepath)

os.system("./test.sh > " + filepath)
## 3
array = []
for i in range(0, 4*4) :
    performance_result = performance_result[performance_result.find("Peak Performance = "):]
    performance_result = performance_result[len("Peak Performance = "):]
    n = int(performance_result[:8])
    array.append(n)


print "3. What was your 4-thread-to-one-thread speedup? \n %s" %

