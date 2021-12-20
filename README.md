# unitn-hpc-seminar
Stuff for UniTN HPC seminar participants

## Examples

### I - Create a simple container

Create a simple container, based on Ubuntu 20.04 image.
The container reads a very simple C++ program (hello_world.cc),
compiles it and store the executable.

Once created, executes the program everytime one runs the
container.

Instructions:

```
1. sudo singularity build hello_world.sif hello_world.def
2. scp hello_world.sif <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) module load singularity-3.4.0
4. singularity run hello_world.sif
```

### II - Create a container using GPUs (--nv switch)

Create a container with PyTorch python library, and use it
to demonstrate usage of "--nv" switch to take advantage of
available GPU hardware.

Instructions:

```
1. sudo singularity build pytorch.sif pytorch.def
2. scp pytorch.sif pytorch_test.py <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) qsub -l walltime=00:10:00 -q short_cpuQ -I
4. (on a certain, non-GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run pytorch.sif
```

Should return

`CUDA libraries are not available`

as expected; but doing this

```
3. (on the cluster) qsub -l walltime=00:10:00 -q short_gpuQ -I
4. (on a certain, GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run pytorch.sif
```

yields the same result, since we forgot to use the "--nv" switch
of singularity run command. Indeed this sequence:

```
3. (on the cluster) qsub -l walltime=00:10:00 -q short_gpuQ -I
4. (on a certain, GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run --nv pytorch.sif
```

returns

`CUDA libraries are available`

### III - Create a container which uses MPI in "hybrid mode"

Create a container with an MPI implementation on board, and launch it
in "hybrid mode", meaning MPI implemented on the host will "talk to"
MPI inside the container and coordinate to make MPI task run.

This is possible thanks to the PMI standard.

Instructions:

```
1. sudo singularity build osu_benchmarks.sif osu_benchmarks.def
2. scp osu_benchmarks.sif osu_*.pbs \ <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) qsub osu_hello.pbs
4. cat osu_hello.err
5. cat osu_hello.out
```

In order to be sure that the code actually ran on the custer, it's possible
to check on which node it was executed:

```
1. JOBID=$(qstat -x -u $USER | tail -n 1 | cut -d "." -f 1)
2. qstat -xf $JOBID | grep exec_host
```

Alternatvely, one could do last step by combining qstat JSON
output capabilities wth a jq query:

`qstat -xf $JOBID -F json | jq --arg jobid $JOBID \ '.Jobs[$jobid+".hpc-head-n1.unitn.it"].exec_host'`

Anyhow, output should be:

`exec_host = hpc-cXX-nodeYY.unitn.it/n+hpc-cZZ-nodeWW.unitn.it/m`

### IV - Create a container that uses the "--bind" switch

A quite complex issue of creating a redistributable library environment,
that depends on PBS itself at compile time, which is available only
on the cluster. How can it be compiled on a personal computer?
By preliminarly downloading, compiling & installing PBS on the
container, at the same version level of the cluster, the issue has
been overcame. Once on the cluster, the container is run with the
"--bind" options, so that it can access the actual PBS installation.

Instructions:

(beware, step 1 may require a lot of time)

```
1. sudo singularity build gromacs.sif gromacs.def
2. scp gromacs.sif <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) module load singularity-3.4.0
4. singularity shell --bind /opt/pbs:/opt/pbs gromacs.sif
```
