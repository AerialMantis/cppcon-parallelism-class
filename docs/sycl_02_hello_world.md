# Parallelism in Modern C++; from CPU to GPU
### Exercise 2: Hello World

---

In this first exercise you will learn:
* How to create and submit a command group.
* How to define a SYCL kernel function.
* How to stream output from a SYCL kernel function.

---

Once you have a queue you can now submit work for the device to execute, and this is done via command groups, which are made up of commands and data dependencies.

1.) Define a command group

Define a lambda to represent your command group and pass it to the submit member function of the queue as follows:

```
myQueue.submit([&](cl::sycl::handler &cgh) {
  
});
```

Note that submitting a command group without any commands will result in an error.

2.) Define a SYCL kernel function

Define a SYCL kernel function via the single_task command within the command group as follows:

```
cgh.single_task<hello_world>([=](){

});
```

Remember to declare a class for your kernel name in the global namespace.

3.) Stream “Hello World!” to stdout from the SYCL kernel function

Construct a stream within the scope of the command group as follows:

```
auto os = cl::sycl::stream{128, 128};
```

Then use the stream you constructed within the SYCL  kernel function to print “Hello world!” as follows:

```
os << “Hello world!” << cl::sycl::endl;
```

4.) Try another command

Instead of single_task try another command for defining a SYCL kernel function (see [SYCL 1.2.1 specification][sycl-specification], sec 4.8.5).


[sycl-specification]: https://www.khronos.org/registry/SYCL/specs/sycl-1.2.1.pdf