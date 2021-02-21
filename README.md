# **h5rnd**
**A random HDF5 container generator based on prufer sequence.** In development often the need arises to have a complex HDF5 dataset with atributes and certain properties to test, measure some charaterestics of an software package. A real life dataset may be overly restritive in licensing or size, possibly diverting attenting focus from the main work -- **h5rnd** fills in this gap by providing scalable random datasets, with preditive characteristics for serieal and parallel HDF5.

# usage:
```
steven@io:~/projects/h5rnd$ ./rndt --help
Usage: rndt [options] 

Optional arguments:
-h --help       shows help message and exits
-v --version    prints version information and exits
-o --output     the new hdf5 will be created/or opened rw [required]
-d --data-sets  number of datasets [default: 100]
-g --groups     number of groups [default: 10]
--hdf5          flag for HDF5 output [default: false]
--graphviz      flag for graphviz output [default: false]
```

![example output](images/tree.svg)

# requirements:

- [h5cpp v1.10.6-2](http://h5cpp.org) included with this project
- [argparse](https://github.com/p-ranav/argparse)  included with this project
- HDF5 C base library, no high level API or beuilt in C++ API is needed works with:  `serial|parallel`
- c++17 or higher compiler

**visualization:**

- graphviz
- xviewer

# bugs
use the included/shipped **h5cpp** header files until I update the original **h5cpp** distribution with `h5::gr_t` support for `read|write|create` operators.