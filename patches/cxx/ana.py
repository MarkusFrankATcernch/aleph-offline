import sys
import os

lines = os.popen("grep "+sys.argv[1]+" *.c").readlines()
files = set()
for l in lines:
    files.add(l.split(':')[0])

for f in files:
    res = os.stat(f)
    print( '%-32s  %8d'%(f, res.st_size, ))
    #
    #os.unlink(f)
