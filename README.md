#Rshell

Rshell is a simple terminal based on unix's bash.
##Installation

>$ git clone https://github.com/UrielNaranjo/rshell.git

>$ cd rshell

>$ git checkout hw0

>$ make

>$ rshell

##Dependencies
The Boost library is required in order to compile this code. 

To install Boost on linux run: 
>$ sudo apt-get install libboost-all-dev
To install on Boost on Mac OS use homebrew and run:
>$ brew install boost

##Bugs/Limitations/Issues
Some known bugs are: 
* Rshell currently cannot support connectors `&&` and `||`
* exit command acts weird after rshell recursively runs itself

