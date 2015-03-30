import os
import string
from subprocess import call
import tempfile


qsub_file = """#!/bin/sh
#PBS -l nodes={nodes}:ppn=12
#PBS -e {directory}/err.txt
#PBS -o {directory}/out.txt

cd {base}/{directory}
mpirun -np {np} /home/ndb245/workspace/homework2/ssort {N} > output.txt
"""

# d = tempfile.mkdtemp(prefix='.tmp.', dir=os.getcwd())
# os.chdir(d)

nodes=6
ppn  = 12
np = nodes * ppn

base = os.getcwd()

Ns = [100000 * tp for tp in [1, 2, 4, 8, 16, 32, 64, 128]]

for N in Ns:
    directory = "job.N%d"%(N)
    os.mkdir(directory)
    os.chdir(directory)
    subfile = "submit.pbs"

    with open(subfile, 'w') as f:
        f.write(qsub_file.format(**locals()))

    os.system('qsub %s'%subfile)

    os.chdir(base)
