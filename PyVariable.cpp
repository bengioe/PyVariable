#include "PyVariable.h"
#include "PyException.h"

PyVariable::PyVariable(){
  m_obj = NULL;
}
PyVariable::PyVariable(PyObject* obj){
  m_obj = obj;
}
PyVariable::~PyVariable(){
  Py_XDECREF(m_obj);
}

PyObject* PyVariable::get(){
  if (m_obj==NULL){
    throw PyException("PyVariable::get","the PyVariable is empty");
  }
  return m_obj;
}
bool PyVariable::isEmpty(){
  return m_obj==NULL;
}


PyVariable PyVariable::new_dict(){
  return PyVariable(PyDict_New());
}
PyVariable PyVariable::new_int(){
  return PyVariable(PyInt_FromLong(0));
}
PyVariable PyVariable::new_int(const long l){
  return PyVariable(PyInt_FromLong(l));
}
PyVariable PyVariable::new_str(){
  return PyVariable(PyString_FromString(""));
}
PyVariable PyVariable::new_str(const char* s){
  return PyVariable(PyString_FromString(s));
}


char* PyVariable::c_str(){
  PyObject* s = PyObject_Str(m_obj);
  char* ret = PyString_AsString(s);
  Py_DECREF(s);
  return ret;
}

std::string PyVariable::str(){
  std::string ret(this->c_str());
  return ret;
}


PyVariable PyVariable::operator+(PyVariable other){
  PyObject* o = other.get();
  PyObject* result = PyNumber_Add(m_obj,o);
  return PyVariable(result);
}
PyVariable PyVariable::operator-(PyVariable other){
  PyObject* o = other.get();
  PyObject* result = PyNumber_Subtract(m_obj,o);
  return PyVariable(result);
}
PyVariable PyVariable::operator/(PyVariable other){
  PyObject* o = other.get();
  PyObject* result = PyNumber_Divide(m_obj,o);
  return PyVariable(result);
}
PyVariable PyVariable::operator*(PyVariable other){
  PyObject* o = other.get();
  PyObject* result = PyNumber_Multiply(m_obj,o);
  return PyVariable(result);
}


void PyVariable::operator=(PyVariable other){
  Py_XDECREF(m_obj);
  m_obj = other.get();
}
void PyVariable::operator=(long other){
  Py_XDECREF(m_obj);
  m_obj = PyInt_FromLong(other);
}
void PyVariable::operator=(const char* other){
  Py_XDECREF(m_obj);
  m_obj = PyString_FromString(other);
}


PyVariable PyVariable::operator[](int index){
  PyObject* pyindex = PyInt_FromLong(index);
  PyVariable ret_value;
  if (PySequence_Check(m_obj)){
    ret_value = PyVariable(PySequence_GetItem(m_obj,pyindex));  
  }
  else if( PyDict_Check(m_obj)){
    if( PyDict_Contains(m_obj,pyindex)){
      ret_value = PyVariable(PyDict_GetItem(m_obj,pyindex));
    }
    else{
      PyDict_SetItem(m_obj,pyindex,PyInt_FromLong(0));
      // return now so not to decref the pyindex
      return PyVariable(PyDict_GetItem(m_obj,pyindex));
    }
  }
  else{
    throw PyException("PyVariable::operator[]","Not a sequence or a dict");
  }
  Py_DECREF(pyindex);
  return ret_value;
}

PyVariable PyVariable::operator[](PyVariable index){
  PyObject* pyindex = index.get();
  PyVariable ret_value;
  if (PySequence_Check(m_obj)){
    ret_value = PyVariable(PySequence_GetItem(m_obj,pyindex));  
  }
  else if( PyDict_Check(m_obj)){
    if( PyDict_Contains(m_obj,pyindex)){
      ret_value = PyVariable(PyDict_GetItem(m_obj,pyindex));
    }
    else{
      PyDict_SetItem(m_obj,pyindex,PyInt_FromLong(0));
      // return now so not to decref the pyindex
      return PyVariable(PyDict_GetItem(m_obj,pyindex));
    }
  }
  else{
    throw PyException("PyVariable::operator[]","Not a sequence or a dict");
  }
  Py_DECREF(pyindex);
  return ret_value;
}
