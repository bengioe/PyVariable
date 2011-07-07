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

You need the Python 2.x headers, usually installed alongside Python itself. Support for Python 3.x is not really guaranteed since functions in the C API were changed &/| deprecated.

Other than that you can either add the few files to your running project or try making a shared object with gcc.