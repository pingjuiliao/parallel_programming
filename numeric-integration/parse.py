#!/usr/bin/python
import os
import sys

if len(sys.argv) < 3 :
    print "Usage: %s <file> <keyword>"
    quit()


filename = sys.argv[1]
keyword  = sys.argv[2]

with open(filename, "rb") as f :
    s = f.read()
    f.close()

numbers = []
while s.find(keyword) >= 0 :
    line = s[s.find(keyword):]
    line = line[:line.find("\n")]
    s = s[s.find(keyword)+len(keyword):]
    array = line.split(" ")
    res = 0.0
    for i in array :
        try :
            res = float(i)
            break
        except :
            continue
    if res :
        numbers.append(res)

print numbers

numt = 4
i = 0
while i < len(numbers) :
    if i % 8 == 7 :
        sys.stdout.write(str(numbers[i]) + "\n")
    else :
        sys.stdout.write(str(numbers[i]) + " ")
    i += 1

#
# import matplotlib.pyplot as plt
# plt.plot()
