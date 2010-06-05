#include <exception>
#include <string>
using namespace std;

class PyException : public exception
{
 public:
  PyException(std::string type,std::string message){
    m_type = type;
    m_mess = message;
  }
  ~PyException() throw(){};
  virtual const char* what() const throw(){
    return ("PyVariableError:\n\t"+m_type+": "+m_mess).c_str();
  }
 private:
  string m_type;
  string m_mess;
};
