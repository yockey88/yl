## Y-Lang

## Hello World
> main.yl
```
main: u32 = (argc: u32 , argv: string[]) {
  print "Hello World!";

  return 0;
}
```

> Run the interpreter
```bash
ylc main.yl
Hello World!
```

## Overview

The Y-Lang interpreter works by parsing the input files, topologically sorting the inputs based on their 'include'/'import' dependency graphs,
constructing a symbol table for the ast which is used to statically type-check and perform basic syntax checking before the AST is interpreted. 

In the future, `ylc` will have a switch to tell it to either interpret the source or compile the program to byte-code

For all the command line options available see [YLC](#ylc-section)

## Syntax

### Comments

Single line comments start with '//' and multine line comments are enclosed within '/* ... */`

```
/// this is a comment

/*
  this comment is
  on multiple lines
*/
```

### Variables

Variables in Y-Lang can not be unitialized

```
a = 2; // a deduced as u8
b: u32 = 100;

c = a + b; /// type deduction will default to the larger of the two types so c is a u32
print c; // 102

str: string = "This is a string"
str2 = "this string's type was deduced"

// chars
c = 'c';
c2: char = 'b';
```

Y-Lang supports signed and unsigned integers of sizes 8, 16, 32, and 64 bytes and will be adding 128-bit integers in the future,
there are also floats and doubles (f32 and f64), strings and characters (string and char), and pointers will be added in the future as well

### Structs

Custom types can be declared using the `struct` keyword.

```
struct Buffer {
  size: u64 = 10;
  data: u8[size] {0};
};

buff: Buffer;
buff.data[5] = 1

print buff; // struct { 10 , [0,0,0,0,0,1,0,0,0,0] }
```

### Functions

The grammer for function declarations is:
`param-list : '(' <identifier> ':' <type> ')'`
`function-decl : 'func'? <identifier> (':' <type>)? '=' <param-list>? '{' <stmt-list> '}'`

```
get_a_number = () {
  return 7;
}

num = get_a_number();
print num; // 7

func filename: string = () {
  return "main.yl";
};

func printstr = (str: string) {
  print(string);
}

func main = (argc: u32 , argv: string[]) {
  printstr(filename()); // main.yl
  return 0;
}
```

Y-Lang also supports closures

```
func get_counter = {
  i = 0;
  return {
    i = i + 1;
    return i;
  };
}

counter = get_counter();
counter2 = get_counter();

print counter(); // 0
print(counter()); // 1, parentheses are optional
print counter2; // 0
print counter(); // 2
print counter2(); // 1
print counter2(); // 2
```

## YLC
<a name="ylc-section"></a>

```
  -h,            --help,                prints help message
  -hm <command>, --help-more <command>, prints detailed help for the command passed to it
  -v,            --version,             prints the version
  -d,            --debug,               enables debug output for the compiler (used for development and maintenance)
  -V,            --verbose,             tells the interpreter to be verbose (identical to debug during symbol table build pass)
  -pp,           --preprocess,          only preprocess the output and write out to "<input>.ylp" file 
  -l,            --lex,                 prints all tokens lexed out to "<input>.ylt" file
  -p,            --parse,               prints string representatino of abstract syntax tree to "<input>.ast" file
  -o <name>,     --output <name>,       changes all output file names to passed <name>
  -I <path>,     --include <path>,      adds <path> to include search paths
  -f,            --force,               tells relevant commands to overwrite paths if they exist already
  new <name>,                           creates a new project in directory <name> 
  build <path>,                         builds the project in the current directory if <path> is not specified, otherwise attempts to build project at <path>
  run <path>,                           runs the project in the current directoy if <path> is not specified, otherwise attempts to run project at <path>
```
