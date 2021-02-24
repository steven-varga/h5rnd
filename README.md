# **h5rnd**
**A random HDF5 container generator based on Prüfer sequence.** In development often the need arises for a complex HDF5 dataset with atributes and certain properties to test, measure some charaterestics of a software package. A real life dataset may be restritive in licensing or size, possibly diverting focus from the main work -- **h5rnd** fills in this gap by providing scalable random datasets, with predictive characteristics for serial and parallel HDF5.

# usage:
```
Usage: h5rnd [options] 

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
- HDF5 C base library, no high level API or built in C++ API is needed works with:  `serial|parallel`
- c++17 or higher compiler

**visualization:**

- graphviz
- xviewer

```
h5rnd --graphviz -o tree.gv -d 30 -g 11  # create a graphviz output
dot -Tsvg tree.gv -o tree.svg            # convert `dot` file to `svg`
xviewer -wf  tree.svg                    # view it on screen
```

# bugs
use the included/shipped **h5cpp** header files until I update the original **h5cpp** distribution with `h5::gr_t` support for `read|write|create` operators.