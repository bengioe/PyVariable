PyVariable
==========

I first started writing this code out of frustration of having to make huge functions using the Python C API for very simple tasks. Of course, I was aware of the existing tools such as boost::python, but hey, making your own tools is always fun!

Intent
------

PyVariable's first intent was to provide a C++ class that would act as Python object would, to a certain extent. To that purpose, a PyVariable instance tries to be anything a PyObject* can be, and uses the right functions of the Python C API to acheive the intended purpose, hopefully correctly following the POLE.

Example
-------

Check out [test.cpp](https://github.com/bengioe/PyVariable/blob/master/test.cpp)!


Installing and Stuff
--------------------

You need the Python 2.x headers, usually installed alongside Python itself. Support for Python 3.x is really not guaranteed since functions in the C API were changed &/| deprecated.

Other than that you can either add the few files (2 .cpp/2 .h) to your running project or try making a shared object with gcc.

Tips and Bits
-------------

- Try not to pass `PyVariable*`'s around. `PyVariable` objects contain only a `PyObject*` (and some other stuff, see next point) and thus are almost only as big as pointers. Use references if you like. Plus, reference count is well managed within `PyVariable`, so you shouldn't be afraid of copies and other passing around.
- Sometimes methods `malloc` data which shouldn't be freed when those methods return (such as `c_str()`). This data is freed when the `PyVariable` is freed/deleted (not the `PyObject` within).
- You can pass 3 types of C functions to Python:
   - `PyObject* (*)(PyObject* self,PyObject* args)`
   - `void (*)(PyVariable args)`
   - `PyVariable (*)(PyVariable args)`

  `args` will always be a tuple containing the arguments passed.

- Please report any bugs you see, with minimal code if possible!