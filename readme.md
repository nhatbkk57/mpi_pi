# Tuturial to compile and launch MPI C program on Chaos Cluster

### Convention 
- (access): mean on the access node (or login node)
- (compute): mean on the compute node which is provided by interactive or passive job request.

### Step by step:
- Login into Chaos Cluster:
```
ssh -p 8022 user_name@access-chaos.uni.lu
```
- Clone this repository on the access node:
```
(access) git clone https://github.com/nhatbkk57/mpi_pi.git
(access) cd mpi_pi
```
#### For Interactive Mode:
- Request a interactive job with "1 node having 8 core in 1 hour":
```
(access) oarsub -I -l nodes=1/core=8,walltime=1
```
##### Now, we are in compute node.
##### For Intel MPI
- Create build directory
```
(compute) cd ~/mpi_pi
(compute) mkdir build.intel && cd build.intel
```
- Load neccessary modules to compile:
```
(compute) module spider MPI
(compute) module load toolchain/intel
(compute) module list
# The result should be look like this:
# Currently Loaded Modules:
#  1) compiler/GCCcore/6.3.0                   4) compiler/ifort/2017.1.132-GCC-6.3.0-2.27                 7) toolchain/iimpi/2017a
#  2) tools/binutils/2.27-GCCcore-6.3.0        5) toolchain/iccifort/2017.1.132-GCC-6.3.0-2.27             8) numlib/imkl/2017.1.132-iimpi-2017a
#  3) compiler/icc/2017.1.132-GCC-6.3.0-2.27   6) mpi/impi/2017.1.132-iccifort-2017.1.132-GCC-6.3.0-2.27   9) toolchain/intel/2017a
```
- Now, compile the code:
```
(compute) mpiicc ../pi_trapezium_par.c -o pi_trapezium_par
```
- And lauch the program with 8 processor:
```
(compute) mpirun -n 8 ./pi_trapezium_par
```

##### For Open MPI
- Create build directory
```
(compute) cd ~/mpi_pi
(compute) mkdir build.open && cd build.open
```
- Load neccessary modules to compile:
```
(compute) module purge
(compute) module spider OpenMPI
(compute) module load mpi/OpenMPI
(compute) module list
# The result should be look like this:
# Currently Loaded Modules:
#  1) compiler/GCCcore/6.3.0              3) compiler/GCC/6.3.0-2.28              5) system/hwloc/1.11.7-GCC-6.3.0-2.28
#  2) tools/binutils/2.28-GCCcore-6.3.0   4) tools/numactl/2.0.11-GCCcore-6.3.0   6) mpi/OpenMPI/2.1.1-GCC-6.3.0-2.28
```
- Now, compile the code:
```
(compute) mpicc ../pi_trapezium_par.c -o pi_trapezium_par
```
- And lauch the program with 8 processor:
```
(compute) mpirun -np 8 pi_trapezium_par
```

#### For Passive Mode:
(updating)
