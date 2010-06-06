#include "PyDictionnary.h"
#include "PyVariable.h"

PyDictionnary::PyDictionnary(PyObject* o){
	m_obj=o;
}

void PyDictionnary::set(PyVariable k,PyVariable val){
	PyDict_SetItem(m_obj,k.get(),val.get());
}
