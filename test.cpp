#include "PyVariable.h"
#include "PyException.h"


PyObject*  helloFromC(PyObject*,PyObject*);
void       yarrFromC(PyVariable);
PyVariable fastSum(PyVariable);

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
  printf("b = %s\n",b.c_str()); // HELLO WORLD!HELLO WORLD!


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

  // One can easily grab builtin functions:
  PyVariable len = PyVariable::exec("len");
  int b_len = len(b); //len(b) returns a PyVariable, but those
  // can be cast directly to most numeric and string types
  printf("len(b) -> %d\n",b_len);// 24

  // You can call functions that take up to 3 arguments
  // normally too. Anything (any "C" type) you can build a PyVariable with
  // can be used as an argument.
  printf("foo(12) -> %s\n",foo(12).c_str()); // 144
  foo = PyVariable::exec("lambda x,y:x+y");
  printf("foo(28,14) -> %s\n",foo(28,14).c_str());// 42
  
  // You can also pass PyVariables as locals in an evaluation context
  a = 32;
  PyVariable inner_z = 68;
  // Syntax is exec(command, comma separated variable names, PyVariable* ...)
  b = PyVariable::exec("foo(a,z)","a,z,foo",&a,&inner_z,&foo);
  printf("foo(32,68) -> %s\n",b.c_str());// 42
  

  // You can creat dict's, and tuples and lists
  b = PyVariable::dict();
  // Since we don't want to mess with temporary values and incomplete
  // map keys, to set a dictionnary item, one must use the setitem method
  b.setitem("Yet another","test");
  printf("b = %s\n",b.c_str()); // {'Yet another': 'test'}
  
  // Another way to access an attribute, and in case you have a dictionnary at hand
  // is to use getattr.
  PyVariable hasK = b.getattr("has_key")("Yet another");
  printf("b.has_key('Yet another') -> %s\n",hasK.c_str()); // True

  // You can even push the heresy farther and create "Python" functions
  // from proper C functions.
  b = helloFromC;
  b(); // Ahoyhoy from C!
  b("Some",-32,"Arguments");
  // note that helloFromC is a C function defined as 
  // PyObject* helloFromC(PyObject *self, PyObject *args)
  b = yarrFromC;
  b(142);// Yarr! from C with args (142,)
  b();   // Yarr! from C with args ()
  // here, yarrFromC is defined as:
  // void yarrFromC(PyVariable args)
  // args will always be a tuple containing the arguments

  a = "Foo ";
  foo = "Bar!";
  // Remember how we could pass PyVariables as variables to Python?
  // Well our "wrapper" to the C function yarrFromC can as well be passed
  // to Python!
  b = PyVariable::exec("func(a,b)","a,b,func",&a,&foo,&b);

  // Enough with functions, creating maps and dictionnaries on the fly is always
  // fun! The `setitem` method returns a reference to the dictionnary being handled,
  // therefore allowing chaining pushes.
  PyVariable d = PyVariable::dict()
    .setitem("Key 1",42)
    .setitem("Key 2","Value 2")
    .setitem("Other key",yarrFromC);
  printf("%s\n",d.c_str());
  // {'Key 1': 42, 'Key 2': 'Value 2', 'Other key': <_pyvar_functype_void_pv object at 0x...>}

  // You can also build lists on the fly:
  PyVariable l = PyVariable::list()
    .append("Bonzai")
    .append(9001)
    .append(a+foo, 2);
  printf("%s\n",l.c_str()); // ['Bonzai', 9001, 'Foo Bar!', 'Foo Bar!']


  // All in all, these are the function types you can pass to Python:
  // `PyObject* (*)(PyObject* self,PyObject* args)`
  // `void (*)(PyVariable args)`
  // `PyVariable (*)(PyVariable args)`

  // More to come! such as directly calling functions with more than 3 arguments ;)

  return 1;
}

PyObject*
helloFromC(PyObject *self, PyObject *args)
{
  printf("Ahoyhoy from C! %s\n",PyVariable(args).c_str());
  return Py_BuildValue("i", 0);
}

void
yarrFromC(PyVariable args){
  printf("Yarr! from C with args %s\n",args.c_str());
}

PyVariable
fastSum(PyVariable args){
  args = args[0];
  if (PySequence_Check(args.get())){
    int sum = 0;
    int len = args["__len__"]();
    for (int i=0;i<len;++i){
      sum+=args[i].c_int();
    }
    return sum;
  }
  return 0;
}
