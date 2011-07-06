#include "PyException.h"

PyException::PyException(std::string type, std::string message){
    m_type = type;
    m_mess = message;
    PyObject *ptype,*value,*tb;
    PyErr_Fetch(&ptype,&value,&tb);
    if (value!=NULL){
        m_mess+="\n\t  "+PyVariable(ptype)["__name__"].str()+": "+PyVariable(value).str();//("__str__").call().str();
    }
}
