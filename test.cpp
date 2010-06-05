#include "PyVariable.h"
#include "PyException.h"

int main(){
  Py_Initialize();
  
  PyVariable a;
  a = "Hello";
  PyVariable b = PyVariable::new_str(" you!");
  b = a+b;
  b = a[0];
  printf("%s\n",b.c_str());
  return 1;
}
