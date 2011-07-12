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
    return ((std::string)("PyVariableError:\n\t"+m_type+", "+m_mess)).c_str();
  }
 private:
  string m_type;
  string m_mess;
};

#endif
