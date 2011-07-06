#include "PyVariable.h"
#include "PyException.h"

int main(){
  /* First step is always to Initialize the Python interpreter itself
   * Then you can start playing with PyVariable
   */
  Py_Initialize();

  PyVariable a,b;
  a = 12.32; // You can assign most "C" values without problem to a variable
  a = "Hello"; // And reassing to something else
  b = " World!";
  b = a + b; // This calls __add__, so you should be able to 
             // add and multiply most stuff
  printf("%s\n",b.c_str()); // Hello World!
  
  // You can access dictionnary items _AND_ attributes with
  // the [] operator. Accessing a dict's attributes might be 
  // trickier.
  // If a PyVariable is a function, you can call it normally:
  // foo()
  b = b["upper"]();

  PyVariable c = 2;
  b = b*c; // as said earlier, this is just like "string"*n_repetitions in Python
  printf("%s\n",b.c_str()); // HELLO WORLD!HELLO WORLD!

  PyVariable len = PyVariable::exec("len");
  printf("%d\n",len(b).c_int());


  b = PyVariable::new_dict();
  b.set("Yet another","test");
  printf("%s\n",b.c_str()); // {'Yet another': 'test'}
  
  // Another way to access an attribute, in case you have a dictionnary at hand
  // is to use getattr.
  PyVariable hasK = b.getattr("has_key")("Yet another");
  printf("%s\n",hasK.c_str()); // True

  // You can also evaluate Python code, such as handy lambda's
  // quite easily
  b = PyVariable::exec("lambda x:x**2");
  // You can call functions that take up to 3 arguments
  // normally too. Anything (any "C" type) you can build a PyVariable with
  // can be used as an argument
  printf("%s\n",b(12).c_str()); // 144
  b = PyVariable::exec("lambda x,y:x+y");
  printf("%s\n",b(28,14).c_str());// 42

  // More to come! such as calling functions with more than 3 arguments ;)

  return 1;
}
