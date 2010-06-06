#include "PyVariable.h"
#include "PyException.h"

int main(){
  Py_Initialize();

  PyVariable a,b;
  a = "Hello";
  b = " World!";
  b = a+b;
  printf("%s\n",b.c_str());

  b = a[1];
  b = b("upper");
  printf("%s\n",b.c_str());

  b = PyVariable::new_dict();
  b.dict.set("allo","ZOMG!");
  printf("%s\n",b.c_str());
  return 1;
}
