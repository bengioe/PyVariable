#include "PyVariable.h"
#include "PyException.h"

int main(){
  Py_Initialize();

  PyVariable a,b;
  a = "Hello";
  b = " World!";
  b = a+b;
  printf("%s\n",b.c_str()); // Hello World!

  b = b["upper"]();
  printf("%s\n",b.c_str()); // HELLO WORLD!

  b = PyVariable::new_dict();
  b.set("Yet another","test");
  printf("%s\n",b.c_str()); // {'Yet another': 'test'}
  b = PyVariable::exec("lambda x:x**2");
  printf("%s\n",b(12).c_str()); // 144
  b = PyVariable::exec("lambda x,y:x+y");
  printf("%s\n",b(28,14).c_str());// 42
  return 1;
}
