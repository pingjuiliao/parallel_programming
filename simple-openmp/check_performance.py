#!/usr/bin/python
import os

filepath = "./output"
if os.path.exists(filepath) :
    os.unlink(filepath)

os.system("uname -a > " + filepath)

## 1
with open(filepath, "r") as f :
    machine_name_string = f.read()
    f.close()

print "1. What Machine I ran on \n %s" % machine_name_string

## 2
os.system("./test.sh > " + filepath)
with open(filepath, "r") as f :
    performance_result = f.read()
    f.close()

print "2. What performance results did you get? \n %s" % performance_result

## 3
array = []
for i in range(0, 4*4) :
    performance_result = performance_result[performance_result.find("Peak Performance = "):]
    performance_result = performance_result[len("Peak Performance = "):]
    n = int(performance_result[:8])
    array.append(n)


print "3. What was your 4-thread-to-one-thread speedup? \n %s" %

