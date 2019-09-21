# Parallelism in Modern C++; from CPU to GPU
### Exercise 1: Configuring a Queue

---

In this first exercise you will learn:
* How to construct a queue using a device selector.
* How to define and use your own device selector.
* How to query information about a device.

---

The first thing you must do in a SYCL application is to construct a queue that will enqueue work, and a queue is associated with a single device to which it enqueues work. The simplest way to construct a queue in SYCL is to pass it a device selector, that is then used to choose a device from all the devices available in your system.

1.) Create a queue using the default selector

Construct a queue using the default selector as follows:

```
auto myQueue = cl::sycl::queue{cl::sycl::default_selector{}};
```

2.) Print the name of the device that is chosen

Retrieve the chosen device from your queue and query the name of the device as follows:

```
auto myDevice = myQueue.get_device();
auto deviceName = myDevice.get_info<cl::sycl::info::device::name>();
std::cout << Chosen device: << deviceName << std::endl;
```

3.) Try other device selectors

Replace the default selector with one of the other standard device selectors that are provided by SYCL (see [SYCL 1.2.1 specification][sycl-specification], sec. 4.6.1.2).

4.) Create your own device selector

Create a device selector using the template below, implementing the function call operator, using various device info queries like the one we used earlier to query the device name (see SYCL 1.2.1 specification, sec. 4.6.4.2) and then use that device selector in the queue constructor:

```
class my_device_selector : public device_selector {
 public:
  my_device_selector() {}

  virtual int operator()(const device &device) const { }
};
```

Remember that the value returned will reflect the score for each device, and a device with a negative score will never be chosen. 


[sycl-specification]: https://www.khronos.org/registry/SYCL/specs/sycl-1.2.1.pdf