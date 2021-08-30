#!/usr/bin/env python

import sys
import subprocess

if (len(sys.argv)!=2) :
    print("ERROR: this script expects exactly 1 argument")
    print("Variables: "+sys.argv[1])
    sys.exit(0)

nEntries = sys.argv[1]

print("Number of entries to process: "+nEntries)

scriptSpecs = 'Analyze.C+('+ str(nEntries)+')'
print(scriptSpecs)
rootCommand = ['root']
rootCommand.append('-b')
rootCommand.append('-q')
rootCommand.append(scriptSpecs)

(out,err) = subprocess.Popen(rootCommand,stdout=subprocess.PIPE).communicate()
print(out)
print("Any errors?")
print(err)
