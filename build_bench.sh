#!/bin/bash


#SBATCH --cpus-per-task=40
#SBATCH --export=ALL
#SBATCH --job-name="swbench"
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes

#SBATCH --nodes=1
#SBATCH --output="swbench.%j.%N.out"
#SBATCH -t 2:00:00
#SBATCH --constraint=cascade



### This scripts builds the swiftware benchmark on Niagara server (and possibly other linux machines). 
### It first installs PAPI and then builds the repository


module load NiaEnv/.2022a
module load intel/2022u2
export MKL_DIR=$MKLROOT
module load cmake
module load gcc


echo "---- Installing PAPI ----"
# Install PAPI library
#git clone https://bitbucket.org/icl/papi.git  
git clone https://github.com/icl-utk-edu/papi.git
cd papi/src
mkdir -p -- ${HOME}/programs/papi
./configure --prefix=${HOME}/programs/papi/
make
make install
cd ../../


echo "----- Building swbench -----"
# Create build folder
mkdir -p build && cd build

# Configure
cmake -DPROFILING_WITH_PAPI=ON -DCMAKE_BUILD_TYPE=Release -DPAPI_PREFIX=${HOME}/programs/papi/ ..

# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Release -- -j4

echo "---- running an example ----"

./example/gemvwithPAPI



