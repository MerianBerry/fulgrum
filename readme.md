<h1>
  <center>
  Fulgra
  </center>
</h1>

## Modules

Modules are the representation of source files. You can import other source files as modules, and be able to use their symbols. Using the `reserve` keyword before a symbol declaration, that symbol will not be shared through the module.

There is only one module per source file, meaning if two or more source files import a module, they share the same module instance.

If the output from `import` inst stored in a variable, the module members will become global in the file, and accessable without a member access (`main_var.member_symbol`).

## Syntax

### Functions

For an indefinate number of versions, there arent first class functions, only function pointers/references. This is because first class functions create a lot of semantic issues that I dont wnat to deal with.

### Examples

A quick program that prints the first argument handed to it (command line arguments). If there is no first argument, the program will say so.

```js
class state {
  public argc: int
  public argv: list(string)
  public ret: int
  public func printFirstArg() => int {
    if @argv[1] == nil {
      print(`There is no first argument\n`)
      @ret = 1
      return 1
    }
    else {
      print(`The first argument is ${@argv[1]}\n`)
      @ret = 0
      return 0
    }
  }
  state(argc: nil, int, argv: nil, list(string)) {
    @argc = (argc == nil) ? 0 : argc
    @argv = argv
  }
}

func main(argc: int, argv: list(string)) => int {
  var myState: state = state(argc)
  myState.argv = argv

  if myState.printFirstArg() != 0 {
    print("Failed to print the first argument!\n")
  }
  return myState.ret
}
```

A mostly identical non-typesafe version of the first example

```js
class state {
  public argc
  public argv
  public ret
  public func printFirstArg() {
    if @argv.size() <= 1 {
      print "There is no first argument\n"
      @ret = 1
      return @ret
    }
    else {
      print `The first argument is ${@argv[1]}\n`
      @ret = 0
      return @ret
    }
  }
  state(argc, argv) {
    @argc = argc
    @argv = argv
  }
}

func main(argc, argv) {
  var myState = state(argc)
  myState.argv = argv

  if myState.printFirstArg() != 0 {
    print "Failed to print the first argument!\n"
  }
  return myState.ret
}
```

An example of code importing

```js
// sourceA.bloss
class classA {
  private num: float
  public getNum() => float {
    return @num
  }
  public setNum(num: float) {
    @num = num
  }
  A(num: nil, float) {
    @num = (num == nil) ? 0 : num
  }
}

func main(num: float) => classA {
  return classA(num)
}
```

```js
// sourceB.bloss
var moudleA: module = import "sourceA"


func main(argv) {
  var varA: moduleA.classA = moduleA.main(3.14)

  print `float from classA: ${varA.getNum()}\n`
}
```

The output:

```none
float from classA: 3.14000
```

### Typing

Fulgra is a strongly typed language, with inferred types, and static type checking.

The idea of typing revolves around conformity. All variables conform to their possible value types, unless the variable forces type parity with its value. If the variable forces type parity, and any value that is set to that variable can't match the expected type, then an error will occur.

How this works: Variables are callled amalgams, where all possible types that their values will have are searched, and the size of the variable in the stack is equal to the largest type size that it will be exposed to (Yes this does mean arguments will be passed to functions through the stack, which is not the practice in some calling conventions). 

This system will be able to produce weak-like typing, without any runtime checks, or even needed a standard library. But for sizes greater than 8 bytes, a builtin memcpy will be compiled into the code and used to copy larger sections of data.

Classes have whats called "infer functions", which are functions that can be used to convert the class data into another type. Infer functions are explicitly defined by the user, and do not modify the data of the value that they are associated with, unless explicitly defined to do so by the user.

Vectors will have one amalgam type. Each element will be a part of this amalgam type if possible. The element with the largest type size will be used for each element type size. This means if you have a vector of integers, but have a class the size of 20 bytes, each element type will be 20 bytes in size. While this can increase memory usage, it isnt a big issue in modern systems. But if memory usage is a big deal, Fulgra supplies ways to define the vector element type.

```js
func main(argc: int, argv: list(string)) => int {
  class state {
    public argc: int
    public argv: list(string)
    public ret: int
    public func printFirstArg() => int {
      if @argv[1] == nil {
        print(`There is no first argument\n`)
        @ret = 1
        return 1
      }
      else {
        print(`The first argument is ${@argv[1]}\n`)
        @ret = 0
        return 0
      }
    }
    state(argc: nil, int, argv: nil, list(string)) {
      @argc = (argc == nil) ? 0 : argc
      @argv = argv
    }
  }

  var myState: state = state(argc)
  myState.argv = argv

  if myState.printFirstArg() != 0 {
    print("Failed to print the first argument!\n")
  }
  return myState.ret
}

// Nowhere near type safe. but it still works and will act the same as the first example
func main(argc, argv) {
  class state {
    public argc
    public argv
    public ret
    public func printFirstArg() {
      print `The first argument is ${@argv[1]}\n`
      @ret = @argv[1] != nil
      return @ret
    }
    state(argc, argv) {
      @argc = argc
      @argv = argv
    }
  }
  
  var myState = state(argc)
  myState.argv = argv

  if myState.printFirstArg() != 0 {
    print "Failed to print the first argument!\n"
  }
  return myState.ret
}

// Variable with the infered type of an integer.
var myVar = 56

// myFunc requires `myParam` to be a type that can have an integer added to it.
func myFunc(myParam, myNumberParam: int) {

  return myParam + myNumberParam
}

// Conformant.
myFunc(myVar, 5)

// Conformant (concatenation).
myFunc("this is a string", 5);

// Non-conformant, cannot add a number to a vector.
myFunc([4, 5, 6], 5)

// Also.
// Invalid. second parameter `myNumberParam` requires a type that can be converted to 
// an integer, and string cannot be converted to an integer.
myFunc("this is a string", "this is also a string")


// This `myFunc` requires the parameter type be an integer or a string.
// String type allows the addition of integers via concatenation.
func myFunc(myConformantParam: int, string) {
  return myConformantParam + 56
}

// Valid
myFunc(5)

// Valid
myFunc("hello world!")

// Invalid. Vector type cant become a number or string.
myFunc([1, 2, 3])


class myClass {
  myInt: int
  myClass(myIntParam: int) {
    @myInt = myIntParam
  }
  infer func __toInt() => int {
    return @myInt
  }
  infer func __toString() => string {
    return (string)@myInt
  }
}

var myClassVar: myClass = myClass(56)

/* 
 * Valid. `myClass` has an infer function `__toInt`, which returns an integer.
 * The compiler will look in a class to see all its infer functions, 
 * and use the respective function that will get the expected type result
 * In this case, the compiler sees `__toInt`, and knows the parameter type is an
 * integer, so the code will call `__toInt`, and use the return value
 * for the parameter of myFunc.
*/
myFunc(myClassVar)

// Also valid. myClass's member function `__toInt` returns an integer,
// which respects the type parity.
myFunc(myClassVar.__toInt())
```
