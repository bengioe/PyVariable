#include "PyVariable.h"
#include "PyException.h"

PyObject* _pyvar_call_void_pv(PyObject* func,PyObject* args,PyObject* kw){
  PyCFunction meth = PyCFunction_GET_FUNCTION(func);
  void (*fpFunc)(PyVariable) = (void (*)(PyVariable))meth;
  PyVariable pvargs = args;
  fpFunc(pvargs);
  return Py_None;
}

PyObject* _pyvar_call_pv_pv(PyObject* func,PyObject* args,PyObject* kw){
  PyCFunction meth = PyCFunction_GET_FUNCTION(func);
  PyVariable (*fpFunc)(PyVariable) = (PyVariable (*)(PyVariable))meth;
  PyVariable pvargs = args;
  PyVariable result = fpFunc(pvargs);
  return result.get();
}

void _pyvar_dealloc_func(PyObject* fp){
  // nothing to free for us
}

static PyTypeObject _pyvar_functype_void_pv = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Cfunc(void(*)(PyVariable))", /*tp_name*/
    sizeof(PyObject),          /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    _pyvar_dealloc_func,       /*tp_dealloc*/
    /*tp_print,tp_getattr,tp_setattr,tp_compare,tp_repr,
      tp_as_number,tp_as_sequence,tp_as_mapping,tp_hash*/
    0,0,0,0,0,0,0,0,0,
    _pyvar_call_void_pv,       /*tp_call*/
    /*tp_str,tp_getattro,tp_setattro,tp_as_buffer*/
    0,0,0,0,
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "PyVariable function (void (*)(PyV&))",/* tp_doc */
};

static PyTypeObject _pyvar_functype_pv_pv = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Cfunc(PyVariable(*)(PyVariable))", /*tp_name*/
    sizeof(PyObject),          /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    _pyvar_dealloc_func,       /*tp_dealloc*/
    /*tp_print,tp_getattr,tp_setattr,tp_compare,tp_repr,
      tp_as_number,tp_as_sequence,tp_as_mapping,tp_hash*/
    0,0,0,0,0,0,0,0,0,
    _pyvar_call_pv_pv,         /*tp_call*/
    /*tp_str,tp_getattro,tp_setattro,tp_as_buffer*/
    0,0,0,0,
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "PyVariable function (PyV (*)(PyV))",/* tp_doc */
};



PyVariable::PyVariable() {
    m_obj = NULL;
    m_flag = 42;
}

PyVariable::PyVariable(const PyVariable& o){
  m_obj = o.m_obj;
  m_flag = o.m_flag;
  Py_XINCREF(m_obj);
}

PyVariable::PyVariable(PyObject* obj, bool isBorrowedRef) {
    /* Borrowed References are when we get a PyObject* we do not "own", that is,
     * we are not responsible to "free" it, or to decref it.
     * One can either use a flag or just increase the reference count, and then
     * automatically decref any PyObject in a PyVariable. The second is what is
     * done here.
     */
    if (isBorrowedRef){
        Py_XINCREF(obj);
    }
    m_obj = obj;
    m_flag = 42;
}

PyVariable::PyVariable(const char* s) {
    m_obj = PyString_FromString(s);
    m_flag = 42;
}

PyVariable::PyVariable(std::string s) {
    m_obj = PyString_FromString(s.c_str());
    m_flag = 42;
}

PyVariable::PyVariable(int i) {
    m_obj = PyInt_FromLong(i);
    m_flag = 42;
}

PyVariable::PyVariable(long i) {
    m_obj = PyInt_FromLong(i);
    m_flag = 42;
}

PyVariable::PyVariable(double d) {
    m_obj = PyFloat_FromDouble(d);
    m_flag = 42;
}

PyVariable::PyVariable(PyObject* (*fpFunc)(PyObject*,PyObject*)){
    //this is bad and will cause a memory leak because there won't be
    //any track of methd, so it can't be freed...
    PyMethodDef* methd = new PyMethodDef();
    methd->ml_name = "pyvar_func";
    methd->ml_meth = fpFunc;
    methd->ml_flags= METH_VARARGS;
    methd->ml_doc  = NULL;
    PyObject* name = PyString_FromString(methd->ml_name);
    m_obj = PyCFunction_NewEx(methd,NULL,name);
    Py_DECREF(name);
    m_flag = 42;
}

PyVariable::PyVariable(void (*fpFunc)(PyVariable)){
    PyMethodDef* methd = new PyMethodDef();
    methd->ml_name = "pyvar_func";
    methd->ml_meth = (PyObject* (*)(PyObject*,PyObject*))fpFunc;
    methd->ml_flags= METH_VARARGS;
    methd->ml_doc  = NULL;
    PyObject* name = PyString_FromString(methd->ml_name);
    m_obj = PyCFunction_NewEx(methd,NULL,name);
    Py_DECREF(name);
    // give our own function object type
    m_obj->ob_type = &_pyvar_functype_void_pv;
    m_flag = 42;
}

PyVariable::PyVariable(PyVariable (*fpFunc)(PyVariable)){
    PyMethodDef* methd = new PyMethodDef();
    methd->ml_name = "pyvar_func";
    methd->ml_meth = (PyObject* (*)(PyObject*,PyObject*))fpFunc;
    methd->ml_flags= METH_VARARGS;
    methd->ml_doc  = NULL;
    PyObject* name = PyString_FromString(methd->ml_name);
    m_obj = PyCFunction_NewEx(methd,NULL,name);
    Py_DECREF(name);
    // give our own function object type
    m_obj->ob_type = &_pyvar_functype_pv_pv;
    m_flag = 42;
}


PyVariable::~PyVariable() {
    if (m_obj != NULL) {
        if (PyInt_Check(m_obj)) {
            /* "The current implementation keeps an array of integer objects for all
             *  integers between -5 and 256, when you create an int in that range
             * you actually just get back a reference to the existing object.
             * So it should be possible to change the value of 1.
             * I suspect the behaviour of Python in this case is undefined."*/
            int i = c_int();
            if (i < -5 || i > 256) {
                Py_DECREF(m_obj);
            }
        }else{
            Py_XDECREF(m_obj);
        }
    }
    for (std::list<void*>::iterator it = m_tofree.begin(); it != m_tofree.end(); ++it) {
        //printf("freeing stuff from %p %s\n",this,*it);
        free(*it);
    }
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

bool PyVariable::isNone(){
    return m_obj == Py_None;
}

PyVariable PyVariable::setitem(PyVariable k,PyVariable val) {
  if (PyDict_Check(m_obj)){
    PyDict_SetItem(m_obj,k.get(),val.get());
  } else{
    throw PyException("PyVariable::setitem","Object is not a dictionnary");
  }
  return *this;
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
 */
const char* PyVariable::c_str() {
    if (m_obj != NULL) {
        PyObject* s = PyObject_Str(m_obj);
	// "The pointer refers to the internal buffer of string, not a copy."
        char* ret = PyString_AsString(s);
	size_t len = strlen(ret);
	char* local = (char*)malloc(len+1);
	memcpy(local,ret,len);
        local[len]=0;
        Py_DECREF(s);
        //printf("alloc %p %d %s\n",local,len,local);
	m_tofree.push_back((void*)local);
        //printf("tofree %p\n",this);
        return local;
    } else {
        return "(null)";
    }
}

/**
 * @return the string value of this PyVariable using PyObject_Str
 */
std::string PyVariable::str() {
  if (m_obj != NULL) {
        PyObject* s = PyObject_Str(m_obj);
	std::string local(PyString_AsString(s),PyString_Size(s));
        Py_DECREF(s);
        return local;
    } else {
        return "(null)";
    }
}

int PyVariable::c_int() {
    return PyInt_AsLong(m_obj);
}


PyVariable PyVariable::append(PyVariable e, int nrepeat){
  if (PyList_Check(m_obj)){
    for (int i=0;i<nrepeat;i++){
      PyList_Append(m_obj,e.get());
    }
  }else{
    throw PyException("PyVariable::append","Object is not a list. Cannot append.");
  }
  return *this;
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
    if (m_obj!=NULL){Py_XDECREF(m_obj);}
    m_obj = other.get();
    m_flag = other.m_flag;
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
            ret_value = PyVariable(PyDict_GetItem(m_obj, pyindex),true);
        } else {
            throw PyException("PyVariable::operator[]", "Dict has no such key: "+key);
        }
    } else {
          PyObject* o = PyObject_GetAttr(m_obj, pyindex);
	  if (o == NULL) {
              o = PyObject_GetItem(m_obj,pyindex);
              if (o == NULL){
                throw PyException("PyVariable::operator[]", "Object not a dict, and no such attribute "+key+" in object.");
              }
	  }
	  ret_value = PyVariable(o,true);
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
        ret_value = PyVariable(PySequence_GetItem(m_obj, index.c_int()),true);
    } else if (PyDict_Check(m_obj)) {
        if (PyDict_Contains(m_obj, pyindex)) {
            ret_value = PyVariable(PyDict_GetItem(m_obj, pyindex),true);
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


PyVariable PyVariable::dict() {
    return PyDict_New();
}

PyVariable PyVariable::list() {
    return PyList_New(0);
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

PyVariable PyVariable::exec(const char* str,const char* format,...) {
    PyVariable locals = PyVariable::dict();
    PyVariable elems = format;
    elems = elems["split"](",");
    int num = elems["__len__"]();

    va_list arguments;
    va_start ( arguments, num );
    for ( int x = 0; x < num; x++ ){
        PyVariable* arg = va_arg ( arguments, PyVariable* );
	// increase ref because PyDict_SetItem doesn't, so when
	// `locals` is freed, it decrefs it's arguments because they are supposed
	// to be borrowed references
	Py_XINCREF(arg->get());
        locals.setitem(elems[x],arg->get());
    }
    va_end ( arguments );
    return PyVariable(PyRun_String(str,Py_eval_input,PyEval_GetBuiltins(),locals.get()));
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
