# CppCon 2018: Parallel Programming with Modern C++: from CPU to GPU

This repository provides set up instructions and the exerises for the CppCon parallel programing class.

## Pre-requisites

The majority of the exercises will require a only standard C++17 compliant compiler, however if for any reason you cannot use a C++17 compiler, using online compilers such as https://godbolt.org/ and https://wandbox.org/ will suffice.

Some later exercises, those which involve programming for the GPU will have some further dependencies. You will need to install OpenCL drivers for the GPU on your laptop and ComputeCpp SYCL (see instructions below). If you do not have a GPU on your laptop or cannot find suitable OpenCL drivers for your GPU then the CPU will suffice. If you have trouble setting this up or for any reason are unable to install the requirements on your laptop we are providing a docker image which will provide OpenCL drivers for Intel CPU and ComputeCpp SYCL (see instructions below).

## Instructions

### Installing OpenCL drivers:

* You will need the OpenCL drivers for your GPU:

  * Intel OpenCL drivers: https://software.intel.com/en-us/articles/opencl-drivers.
  * AMD OpenCL drivers: https://www.amd.com/en/support.
  * NVidia OpenCL drivers: https://developer.nvidia.com/cuda-toolkit-32-downloads

### Installing ComputeCpp:

* You can download ComputeCpp SYCL and find many useful resources at https://developer.codeplay.com/.

* For help getting setup and verifying your installation see https://developer.codeplay.com/computecppce/latest/getting-started-guide.

* In general ComputeCpp supports Windows 7/10 and Ubuntu 16.04/18.04, and we support Intel CPU/GPU, AMD GPU and have experimental support for NVidia GPU on Ubunutu. For a full list of the supported platforms see https://developer.codeplay.com/computecppce/latest/supported-platforms.

* Note that ComputeCpp currently only supports the earlier Radeon Software Crimson Edition AMD drivers.

### Setting up docker image:

* Install Docker CE from https://docs.docker.com/install/.

* Pull the computecpp_ubunutu1604 docker image:

  `docker pull aerialmantis/computecpp_ubuntu1604`

* Run the computecpp_ubuntu1604 docker image:

  `docker run --rm -it aerialmantis/computecpp_ubuntu1604`

### Preparing this repository

* Once you've got OpenCL and ComputeCpp SYCL setup or you have the docker image setup you can clone this repository to verify your setup and prepare yourself for the class.

* Pull this repository:

  `git clone https://github.com/AerialMantis/cppcon2018-parallelism-class.git`

* Create a build directory:

  `cd cppcon2018-parallelism-class/`
  `mkdir build`
  `cd build`

* Run CMake to configure solution (local setup):

  `cmake ../ -G[Generator] -DCMAKE_BUILD_TYPE=[Debug/Release]`

* Run CMake to configure solution (docker image):

  `cmake ../ -GNinja -DCMAKE_BUILD_TYPE=Debug -DOpenCL_LIBRARY=${OCL_LIB}/libOpenCL.so -DOpenCL_INCLUDE_DIR=${OCL_INC}`

* Note that if you are using an NVidia GPU, in order to use the experimental ComputeCpp SYCL support you must include the following in the above cmake command:

  `-DCOMPUTECPP_BITCODE=ptx64`

* Note that you can disable the SYCL tests in the case you are not able to use ComputeCpp by adding the following in the above cmake command:

  `-DCPPCON_SYCL_EXAMPLES=OFF`

* Build your solution:

  `cmake --build .`

* Verify your setup by running the hello world example:

  `./examples/hello_world`

### Installing GCC 8 (Ubuntu 16.04)

* To install GCC 8 on Ubuntu 16.04 you need update the apt-get repository:

`RUN apt-get install software-properties-common`
`RUN add-apt-repository ppa:ubuntu-toolchain-r/test`
`RUN apt-get update`
`RUN apt-get install g++-8`
