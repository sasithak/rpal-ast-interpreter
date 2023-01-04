# AST Interpreter for RPAL Language

> An interpreter to execute an Abstract Syntax Tree of an RPAL program.

RPAL or Right-reference Pedagogic Algorithmic Language is a simple functional programming language. The interpreter takes an Abstract Syntax Tree (AST) of an RPAL program and executes it. The interpreter is implemented following the [operational semantics](https://rpal.sourceforge.net/doc/semantics.pdf) of the RPAL language and the [CSE rules](https://www.cise.ufl.edu/~manuel/notes/csem.pdf). Following is an AST for a sample program to find whether a given number is positive, negative, or zero.

```
let
.function_form
..<ID:positive_negative>
..<ID:x>
..->
...ls
....<ID:x>
....<INT:0>
...<STR:'Negative'>
...->
....eq
.....<ID:x>
.....<INT:0>
....<STR:'Zero'>
....<STR:'Positive'>
.gamma
..<ID:Print>
..tau
...gamma
....<ID:positive_negative>
....neg
.....<INT:10>
...gamma
....<ID:positive_negative>
....<INT:0>
...gamma
....<ID:positive_negative>
....<INT:10>

```

## Compiling and Running

The project can be compiled by running the following command in the `/src` directory.

```
make
```

It will produce an executable named `myrpal`. To run the interpreter, run the `myrpal` executable with the filename as a command line argument to specify the file containing the AST of the RPAL program.

```
myrpal filename
```

The output of the program will be printed to the standard output.

### Compile with Microsoft C++ toolset on Windows

Install instructions for the toolset are available [here](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170). To compile the project, open a Visual Studio developer command prompt (or powershell) and run the following command in the `/src` directory.

```
nmake -f Makefile.win
```

It will produce an executable named `myrpal.exe`. To run the interpreter, run the `myrpal.exe` executable with the filename as a command line argument to specify the file containing the AST of the RPAL program.

```
myrpal.exe filename
```

The output of the program will be printed to the standard output.

### Additional Command Line Arguments

Following commandline arguments are allowed before the filename:

- `-ast`: Prints the AST of the RPAL program to the standard output.
- `-st`: Prints the ST of the RPAL program to the standard output.
- `-cs`: Prints the Control Structures of the RPAL program to the standard output.
- `-exe`: Prints the Execution of the RPAL program to a file named `exec.txt`.

## Testing

Sample RPAL programs are provided in the `test` directory. The `CppTest` library is used to run tests. It can be installed on Ubuntu using the following command:

```
sudo apt-get install libcppunit-dev
```

To run the tests, compile the interpreter and then run the following command in the `/src` directory.

```
make test
```

The test results will be printed to the standard output.
