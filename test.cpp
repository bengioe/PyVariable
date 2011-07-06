#include "PyVariable.h"
#include "PyException.h"

int main(){
  Py_Initialize();

  PyVariable a,b;
  a = "Hello";
  b = " World!";
  b = a+b;
  printf("%s\n",b.c_str());

  b = b["upper"]();
  printf("%s\n",b.c_str());

  b = PyVariable::new_dict();
  b.set("Yet another"," test");
  printf("%s\n",b.c_str());
  b = PyVariable::exec("lambda x:x**2");
  printf("%s\n",b.c_str());
  //printf("%s\n",(b(
  return 1;
}
