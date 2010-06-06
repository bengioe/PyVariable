#ifndef PYDICT_H
#define PYDICT_H
#include <Python.h>
class PyDictionnary;
class PyVariable;

class PyDictionnary{
private:
public:
	PyObject* m_obj;
	PyDictionnary(){m_obj=NULL;}
	PyDictionnary(PyObject* o);
	void set(PyVariable k,PyVariable val);
};
#endif
