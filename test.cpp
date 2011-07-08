#include "PyVariable.h"
#include "PyException.h"


PyObject* helloFromC(PyObject*,PyObject*);
void yarrFromC(PyVariable);

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
  // the [] operator. Accessing a dict's attributes requires 
  // using o.getattr(..) directly
  // If a PyVariable is a function, you can call it normally:
  // foo()
  // here we call b.upper()
  b = b["upper"]();

  PyVariable c = 2;
  // as said earlier, this is just like "string"*n_repetitions in Python
  b = b*c; 
  printf("%s\n",b.c_str()); // HELLO WORLD!HELLO WORLD!

  PyVariable len = PyVariable::exec("len");
  printf("%d\n",len(b).c_int());


  // You can also evaluate Python code, such as handy lambda's
  // quite easily
  PyVariable foo = PyVariable::exec("lambda x:x**2");
  // Note that, as in Python, functions and types and classes are 
  // variables. Also, and that's quite important, notice how earlier
  // we did:
  // b = b["upper"]()
  // b["upper"] isn't just a function object. Somehow Python devs have
  // hacked a brilliant way to attach an object to it's methods, so,
  // b["upper"] can be called, like a callback, and the PyObject* method
  // will "remember" that it is "b" that is the `self` argument when called.

  // You can call functions that take up to 3 arguments
  // normally too. Anything (any "C" type) you can build a PyVariable with
  // can be used as an argument.
  printf("%s\n",foo(12).c_str()); // 144
  foo = PyVariable::exec("lambda x,y:x+y");
  printf("%s\n",foo(28,14).c_str());// 42

  // You can creat dict's, and tuples and lists
  b = PyVariable::new_dict();
  // Since we don't want to mess with temporary values and incomplete
  // map keys, to set a dictionnary item, one must use the setitem method
  b.setitem("Yet another","test");
  printf("%s\n",b.c_str()); // {'Yet another': 'test'}
  
  // Another way to access an attribute, and in case you have a dictionnary at hand
  // is to use getattr.
  PyVariable hasK = b.getattr("has_key")("Yet another");
  printf("%s\n",hasK.c_str()); // True

  // You can even push the heresy farther and create "Python" functions
  // from proper C functions.
  b = helloFromC;
  b(); // Ahoyhoy!
  // note that helloFromC is a C function defined as 
  // PyObject* helloFromC(PyObject *self, PyObject *args)
  b = yarrFromC;
  b(142);// Yarr! (142,)
  // here, yarrFromC is defined as:
  // void yarrFromC(PyVariable args)
  // args will always be a tuple containing the arguments

  // More to come! such as calling functions with more than 3 arguments ;)

  return 1;
}

PyObject*
helloFromC(PyObject *self, PyObject *args)
{
  printf("Ahoyhoy!\n");
  return Py_BuildValue("i", 0);
}

void
yarrFromC(PyVariable args){
  printf("Yarr! %s\n",param.c_str());
}
