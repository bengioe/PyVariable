#include <Python.h>
#include <string>


class PyVariable{
 private:
  PyObject* m_obj;
 public:
  PyVariable();
  PyVariable(PyObject* obj);
  ~PyVariable();

  // Once object is manipulated, 
  // we can send it back to Python:
  PyObject* get();

  // Check wether m_obj is null
  bool isEmpty();
  
  //Creation static methods
  static PyVariable new_dict();
  static PyVariable new_int();
  static PyVariable new_int(long);
  static PyVariable new_str();
  static PyVariable new_str(const char*);
  
  
  //str(o)
  char*        c_str(); // C char*
  std::string  str();   // C++ string

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
  // of o, a new empty object is created for affectation.
  PyVariable operator[](int index);
  //PyVariable operator[](PyObject* index);
  PyVariable operator[](PyVariable index);
  //either o[str] or o.str:
  // if o is not a dict then o[str] acts like o.getattr(str)
  PyVariable operator[](std::string);
};
