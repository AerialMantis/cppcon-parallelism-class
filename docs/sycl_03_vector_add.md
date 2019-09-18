# Parallelism in Modern C++; from CPU to GPU
### Exercise 3: Vector add

---

In this first exercise you will learn:
* How to manage data using buffers
* How to access data using accessors
* How to define an nd range SYCL kernel function

---

In SYCL buffers are used to manage data across the host and device(s), and accessors are used to declare data dependencies to a SYCL kernel function as well as to access the data within a SYCL kernel function.

1.) Allocate your input and output vectors

Allocate memory on the host for your input and output data using std::vectors and initialise the input with values.

```
auto input = std::vector<float>{};
auto output = std::vector<float>{};

input.reserve(size);
output.reserve(size);

std::iota(begin(input), end(output), 0.0f);
std::fill(begin(input), end(output), 0.0f);
```

2.) Construct buffers

Construct a buffer to manage your input and output data.

```
auto inputBuf = cl::sycl::buffer<float, 1>(input.data(),
  cl::sycl::range<1>(intput.size());
auto outputBuf = cl::sycl::buffer<float, 1>(input.data(),
  cl::sycl::range<1>(intput.size());
```

3.) Construct accessors

Construct an accessor for your input and output buffers.

```
auto inputAcc = inputBuf.get_access<cl:sycl::access::mode::read>(cgh);
auto outputAcc = outputBuf.get_access<cl:sycl::access::mode::write>(cgh);
```

4.) Declare your kernel

Declare a SYCL kernel function using the parallel_for command that takes ...

```
cgh.parallel_for<vector_add>(range<1>(input.size()),
  [=](cl::sycl::id<1> id) {
  outputAcc[id] = inputAAcc[id] + inputBAcc[id];
});
```
