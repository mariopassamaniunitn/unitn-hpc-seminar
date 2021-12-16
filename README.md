# unitn-hpc-seminar
Stuff for UniTN HPC seminar participants

= Examples =

I - Create a simple container, based on Ubuntu 20.04 image.
The container reads a simple C++ program (hello_world.cc),
compiles it and store the outcome.

Once created, executes the program everytime one runs the
container.

Instructions:

1. sudo singularity build hello_world.sif hello_world.def
2. scp hello_world.sif <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) module load singularity-3.4.0
4. singularity run hello_world.sif


II - Create a container with PyTorch python library, and use it
to demonstrate usage of "--nv" switch to take advantage of
available GPU hardware.

1. sudo singularity build pytorch.sif pytorch.def
2. scp pytorch.sif pytorch_test.py <your_username>@hpc-head-n1.unitn.it:.
3. (on the cluster) qsub -l walltime=00:10:00 -q short_cpuQ -I
4. (on a certain, non-GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run pytorch.sif

Should return

CUDA libraries are not available

as expected; but doing this

3. (on the cluster) qsub -l walltime=00:10:00 -q short_gpuQ -I
4. (on a certain, GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run pytorch.sif

yields the same result, since we forgot to use the "--nv" switch
of singularity run command. Indeed this sequence:

3. (on the cluster) qsub -l walltime=00:10:00 -q short_gpuQ -I
4. (on a certain, GPU capable node) module load singularity-3.4.0
5. cat pytorch_test.py | singularity run --nv pytorch.sif

returns

CUDA libraries are available
