#ifndef PYEXCEPTION_H_
#define PYEXCEPTION_H_

#include "PyVariable.h"
#include <exception>
#include <string>

using namespace std;

class PyException : public exception
{
 public:
  PyException(std::string type,std::string message);
  ~PyException() throw(){};
  virtual const char* what() const throw(){
    std::string msg = std::string("PyVariableError:\n\t")+m_type+", "+m_mess;
    sprintf((char*)m_msg,"%s",msg.c_str());
    return (const char*)m_msg;
  }
 private:
  string m_type;
  string m_mess;
  char m_msg[2048];
};

#endif
