#ifndef PYVAR_H
#define PYVAR_H
#include <Python.h>
#include <string>
class PyVariable;
#include "PyDictionnary.h"


class PyVariable
{
private:
    PyObject* m_obj;
public:
    PyDictionnary dict;

    PyVariable();
    PyVariable(PyObject* obj);
    // C++ types
    PyVariable(const char* s);
    PyVariable(std::string s);
    PyVariable(long i);
    PyVariable(double d);
    ~PyVariable();

    // Once object is manipulated,
    // we can send it back to Python:
    PyObject* get();

    // Check wether m_obj is null
    bool isEmpty();

    //Creation static methods
    static PyVariable new_dict();
    static PyVariable new_int();
    static PyVariable new_str();


    //str(o)
    char*        c_str(); // C char*
    std::string  str();   // C++ string


    //C/C++ values
	int c_int();


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
    PyVariable operator[](std::string);
    PyVariable operator[](PyVariable index);

    //o.attr
    PyVariable operator()(std::string attr);
};
#endif
