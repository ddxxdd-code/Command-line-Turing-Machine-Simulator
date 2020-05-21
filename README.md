# Turing Machine
 Console Version
 ### Change Log
 * 2020-05-17 Created this file.
 * 2020-05-21 First working version of this program.
## Introduction
This is an omplementation of Turing machine via commandline input.\
This implementation is c based.\
The program receives instructions from input file.\
Running by first compile all the .c files or use make.\
Then run the program called `turingMachine` follow the configuration file:\
~~~
./turingMachine config.txt
~~~
Or you can pick one stage as the initial stage by
~~~
./turingMachine config.txt initialStage
~~~
Then the program will ask you to write the initial sequence.
The running stage and result will be send to stdout.
The blank tape is set to be represented as `"_"`, 
so there is a fixed character you should use when writing the configuration file.
When there is no transition fit the current status, the machine will halt.
## Configuration File
There are 3 moves:
* Move left: L
* Move right: R
* Stay: S
Each follows one value of change.\
The acceptedfile should be in the form below:\
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
There is nothing right now.
