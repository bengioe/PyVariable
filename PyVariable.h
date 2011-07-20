#ifndef PYVAR_H
#define PYVAR_H
#include <Python.h>
#include <string>
#include <list>
class PyVariable;
#include "PyException.h"

class PyVariable
{
private:
    PyObject* m_obj;
    unsigned int m_flag;
    // list of stuff used by our functions that should be freed someday
    // since they could not be immediatly freed when returned
    std::list<void*> m_tofree;
public:
    PyVariable();
    PyVariable(PyObject* obj,bool isBorrowedRef=false);
    PyVariable(const PyVariable& o);
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
    // Check wether obj is None
    bool isNone();
    // For dict's, set a key to a value
    PyVariable setitem(PyVariable key,PyVariable value);
    // Get attribute (one can also use o[attr] if type(o)!=dict)
    PyVariable getattr(std::string attr);

    /// "Common" methods
    //str(o)
    const char*    c_str(); // C char*
    operator const char*() {return this->c_str();}
    std::string      str(); // C++ string
    operator std::string() {return this->str();}

    //o.append
    PyVariable append(PyVariable e);

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
    static PyVariable dict();
    static PyVariable list();
    static PyVariable new_int();
    static PyVariable new_str();
    static PyVariable new_tuple();
    
    //static class methods
    static PyVariable import(std::string module_name);
    static PyVariable reload(PyVariable module);
    // "exec" runs PyRun_String, so you can use it as both an
    // "eval" method and an "exec" method to print various stuff
    static PyVariable exec(std::string str);

    static PyVariable exec(const char* str, const char* args, ...);
};

/*



class _pyvar_dict_bit{
 public:
  _pyvar_dict_bit(){
    d = PyDict_New();
  }
  _pyvar_dict_bit push(PyVariable k,PyVariable v){
    PyDict_SetItem(d,k.get(),v.get());
    return *this;
  }
  PyObject* pop(){
    return d;
  }
  private:
    PyObject* d;
  };

PyVariable PyVariable::operator=(_pyvar_dict_bit db);
static _pyvar_dict_bit PyVariable::dict();

*/
#endif


