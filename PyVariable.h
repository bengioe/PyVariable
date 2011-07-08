#ifndef PYVAR_H
#define PYVAR_H
#include <Python.h>
#include <string>
class PyVariable;
#include "PyException.h"

typedef PyVariable (*PyVariableFunction)(PyVariable self,PyVariable args);

PyObject*
_pyvariable_callpvf(PyObject *self, PyObject *args);
/*
PyMethodDef _PVFMethods[] = {
    {"callfunc", _pyvariable_callpvf, METH_VARARGS,
     "Call a PyVariableFunction"},
    {NULL, NULL, 0, NULL}
    };*/

extern PyObject* _pvf_module;

class PyVariable
{
private:
    PyObject* m_obj;
    unsigned int m_flag;
public:
    PyVariable();
    PyVariable(PyObject* obj);
    // build from C++ types
    PyVariable(const char* s);
    PyVariable(std::string s);
    PyVariable(int i);
    PyVariable(long i);
    PyVariable(double d);
    PyVariable(PyObject* (*fpFunc)(PyObject*,PyObject*));
    PyVariable(void (*fpFunc)(PyVariable));
    ~PyVariable();

    /// PyVariable specific methods

    // Once object is manipulated,
    // we can send it back to Python:
    PyObject* get();

    // Check wether m_obj is null
    bool isEmpty();
    // For dict's, set a key to a value
    void setitem(PyVariable key,PyVariable value);
    // Get attribute (one can also use o[attr] if type(o)!=dict)
    PyVariable getattr(std::string attr);

    /// "Common" methods
    //str(o)
    const char*    c_str(); // C char*
    operator const char*() {return this->c_str();}
    std::string      str(); // C++ string
    operator std::string() {return this->str();}



    //C/C++ values
    int c_int();
    operator int () {return this->c_int();}


    // math!
    PyVariable operator+(PyVariable other);
    PyVariable operator-(PyVariable other);
    PyVariable operator/(PyVariable other);
    PyVariable operator*(PyVariable other);

    // affectation
    void       operator=(PyVariable other);
    void       operator=(long other);
    void       operator=(const char* other);

    //o[k]
    // note that if the object is a dictionnary, and k is not a key
    // of o, an exception is raised. To affect (o[k] = v), use o.dict.set(k,v)
    PyVariable operator[](int index);
    PyVariable operator[](PyVariable index);
    // the following is either o[k] if o is a dict
    // or o.k if o is not a dict
    PyVariable operator[](std::string);
    PyVariable operator[](const char*);
    
    //o()
    PyVariable operator()();
    //o(arg1,[arg2,[arg3]])
    PyVariable operator()(PyVariable arg1,PyVariable arg2=Py_None, PyVariable arg3=Py_None);

    //Creation static methods
    static PyVariable new_dict();
    static PyVariable new_int();
    static PyVariable new_str();
    static PyVariable new_tuple();
    
    //static class methods
    static PyVariable import(std::string module_name);
    static PyVariable reload(PyVariable module);
    // "exec" runs PyRun_String, so you can use it as both an
    // "eval" method and an "exec" method to print various stuff
    static PyVariable exec(std::string str);
};
#endif
