#include "PyVariable.h"
#include "PyException.h"

PyVariable::PyVariable() {
    m_obj = NULL;
}

PyVariable::PyVariable(PyObject* obj) {
    m_obj = obj;
}

PyVariable::PyVariable(const char* s) {
    m_obj = PyString_FromString(s);
}

PyVariable::PyVariable(std::string s) {
    m_obj = PyString_FromString(s.c_str());
}

PyVariable::PyVariable(int i) {
    m_obj = PyInt_FromLong(i);
}

PyVariable::PyVariable(long i) {
    m_obj = PyInt_FromLong(i);
}

PyVariable::PyVariable(double d) {
    m_obj = PyFloat_FromDouble(d);
}

PyVariable::~PyVariable() {
    //Py_XDECREF(m_obj);
}

PyObject* PyVariable::get() {
    if (m_obj == NULL) {
        throw PyException("PyVariable::get", "the PyVariable is empty");
    }
    return m_obj;
}

bool PyVariable::isEmpty() {
    return m_obj == NULL;
}

void PyVariable::set(PyVariable k,PyVariable val) {
  if (PyDict_Check(m_obj)){
    PyDict_SetItem(m_obj,k.get(),val.get());
  } else{
    throw PyException("PyVariable::set","Object is not a dictionnary");
  }
    
}

PyVariable PyVariable::getattr(std::string attr){
    PyVariable ret_value;
    PyObject* o = PyObject_GetAttrString(m_obj, attr.c_str());
    if (o == NULL) {
      throw PyException("PyVariable::getattr", "No such attribute "+attr+" in object.");
    }
    return PyVariable(o);
}

/**
 * @return the string value of this PyVariable using PyObject_Str
 * This is not a copy, do not free
 */
const char* PyVariable::c_str() {
    if (m_obj != NULL) {
        PyObject* s = PyObject_Str(m_obj);
	// "The pointer refers to the internal buffer of string, not a copy."
        char* ret = PyString_AsString(s);
        Py_DECREF(s);
        return ret;
    } else {
        return "(null)";
    }
}

/**
 * @return the string value of this PyVariable using PyObject_Str
 */
std::string PyVariable::str() {
    std::string ret(this->c_str());
    return ret;
}

int PyVariable::c_int() {
    return PyInt_AsLong(m_obj);
}

PyVariable PyVariable::operator+(PyVariable other) {
    PyObject* o = other.get();
    PyObject* result = PyNumber_Add(m_obj, o);
    return PyVariable(result);
}

PyVariable PyVariable::operator-(PyVariable other) {
    PyObject* o = other.get();
    PyObject* result = PyNumber_Subtract(m_obj, o);
    return PyVariable(result);
}

PyVariable PyVariable::operator/(PyVariable other) {
    PyObject* o = other.get();
    PyObject* result = PyNumber_Divide(m_obj, o);
    return PyVariable(result);
}

PyVariable PyVariable::operator*(PyVariable other) {
    PyObject* o = other.get();
    PyObject* result = PyNumber_Multiply(m_obj, o);
    return PyVariable(result);
}

void PyVariable::operator=(PyVariable other) {
    Py_XDECREF(m_obj);
    m_obj = other.get();
    Py_XINCREF(m_obj);
}

void PyVariable::operator=(long other) {
    Py_XDECREF(m_obj);
    m_obj = PyInt_FromLong(other);
}

void PyVariable::operator=(const char* other) {
    Py_XDECREF(m_obj);
    m_obj = PyString_FromString(other);
}

PyVariable PyVariable::operator[](int index) {
    PyObject* pyindex = PyInt_FromLong(index);
    PyVariable ret_value;
    if (PySequence_Check(m_obj)) {
        if (PySequence_Length(m_obj) > index) {
            ret_value = PyVariable(PySequence_GetItem(m_obj, index));
        } else {
            throw PyException("PyVariable::operator[]", "Index out of range");
        }
    } else if (PyDict_Check(m_obj)) {
        if (PyDict_Contains(m_obj, pyindex)) {
            ret_value = PyVariable(PyDict_GetItem(m_obj, pyindex));
        } else {
            throw PyException("PyVariable::operator[]", "Dict has no such key");
        }
    } else {
        throw PyException("PyVariable::operator[]", "Not a sequence or a dict");
    }
    Py_DECREF(pyindex);
    return ret_value;
}

PyVariable PyVariable::operator[](std::string key) {
    PyObject* pyindex = PyString_FromString(key.c_str());
    PyVariable ret_value;
    if (PyDict_Check(m_obj)) {
        if (PyDict_Contains(m_obj, pyindex)) {
            ret_value = PyVariable(PyDict_GetItem(m_obj, pyindex));
        } else {
            throw PyException("PyVariable::operator[]", "Dict has no such key: "+key);
        }
    } else {
          PyObject* o = PyObject_GetAttrString(m_obj, key.c_str());
	  if (o == NULL) {
	    throw PyException("PyVariable::operator[]", "Object not a dict, and no such attribute "+key+" in object.");
	  }
	  ret_value = PyVariable(o);
    }
    Py_DECREF(pyindex);
    return ret_value;
}

PyVariable PyVariable::operator[](const char* key){
  std::string k(key);
  return (*this)[k];
}

PyVariable PyVariable::operator[](PyVariable index) {
    PyObject* pyindex = index.get();
    PyVariable ret_value;
    if (PySequence_Check(m_obj)) {
        ret_value = PyVariable(PySequence_GetItem(m_obj, index.c_int()));
    } else if (PyDict_Check(m_obj)) {
        if (PyDict_Contains(m_obj, pyindex)) {
            ret_value = PyVariable(PyDict_GetItem(m_obj, pyindex));
        } else {
            throw PyException("PyVariable::operator[]", "Dict has no such key");
        }
    } else {
        throw PyException("PyVariable::operator[]", "Not a sequence or a dict");
    }
    return ret_value;
}


PyVariable PyVariable::operator()(PyVariable arg1,
				  PyVariable arg2,
				  PyVariable arg3){
    int len = 1;
    if (arg2.get()!=Py_None){++len;}
    if (arg3.get()!=Py_None){++len;}
    PyObject* args = PyTuple_Pack(len,arg1.get(),arg2.get(),arg3.get());
    PyObject* o = PyObject_Call(this->get(),args,NULL);
    if (o == NULL){
        throw PyException("PyVariable::operator()","Error during function call");
    }
    Py_XDECREF(args);
    return PyVariable(o);
}



PyVariable PyVariable::operator()(){
    PyObject* o = PyObject_Call(this->get(),PyTuple_New(0),NULL);
    if (o == NULL){
        throw PyException("PyVariable::operator()","Error during function call");
    }
    return PyVariable(o);
}



/////////////////////
// static stuff


PyVariable PyVariable::new_dict() {
    return PyVariable(PyDict_New());
}

PyVariable PyVariable::new_int() {
    return PyVariable(PyInt_FromLong(0));
}

PyVariable PyVariable::new_str() {
    return PyVariable(PyString_FromString(""));
}

PyVariable PyVariable::new_tuple() {
    return PyVariable(PyTuple_New(0));
}


PyVariable PyVariable::exec(std::string str) {
    return PyVariable(PyRun_String(str.c_str(),Py_eval_input,PyEval_GetBuiltins(),0));
}
PyVariable PyVariable::import(std::string module_name){
    PyObject* o = PyImport_ImportModule(module_name.c_str());
    if (o==NULL){
        throw PyException("PyVariable::import","Importing module "+module_name+" failed.");
    }
    return PyVariable(o);
}
PyVariable PyVariable::reload(PyVariable module){
    PyObject* o = PyImport_ReloadModule(module.get());
    if (o==NULL){
        throw PyException("PyVariable::import","Reloading module failed.");
    }
    return PyVariable(o);
}
