# Turing Machine
 A simple Turing machine without graphic interface.
 ### Change Log
 * 2020-05-17 Created this file.
 * 2020-05-21 First working version of this program.
## Introduction
This is an implementation of Turing machine via commandline input.\
This implementation is c based.\
The program receives instructions from input file.
## Compile
compile all the .c files or use make:
~~~
$ make all
~~~
## Run
Run the program called `turingMachine` followed by the configuration file:
~~~
$ ./turingMachine config.txt
~~~
Or you can pick one initial stage by
~~~
$ ./turingMachine config.txt initialStage
~~~
Then the program will ask you to write the initial sequence.
The running stage and result will be send to stdout.
When there is no transition from the current status, the machine will halt.
## Clean the directory
~~~
$ make clean
~~~
## Configuration File Syntax
There are 3 moves:
* Move left: L
* Move right: R
* Stay: S

The acceptable file should be in the form below:
~~~
<present_state> <read_character> <write_character> <move_direction> <next_state>
~~~
Below is a sample of such file:
~~~
s1 0 0 r s2
s2 1 0 l s2
s2 _ 1 s s1
~~~
## Comments
* The blank tape is set to be represented as `"_"`,\
so there is a fixed character you should use when writing the configuration file.
