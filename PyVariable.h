#ifndef PYVAR_H
#define PYVAR_H
#include <python2.6/Python.h>
#include <string>
class PyVariable;
#include "PyDictionnary.h"
#include "PyException.h"

class PyVariable
{
private:
    PyObject* m_obj;
public:
    PyDictionnary dict;

    PyVariable();
    PyVariable(PyObject* obj);
    // build from C++ types
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


    //str(o)
    char*        c_str(); // C char*
    operator char*       () {return this->c_str();}
    std::string  str();   // C++ string
    operator std::string () {return this->str();}



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
    PyVariable operator[](std::string);
    PyVariable operator[](PyVariable index);

    //o.attr
    PyVariable operator()(std::string attr);

    //o()
    PyVariable call();
    //o(args)
    PyVariable call(PyVariable args);

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
