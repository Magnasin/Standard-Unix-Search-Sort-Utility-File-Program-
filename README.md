# Standard-Unix-Search-Sort-Utility-File-Program

Used to implement functions by chaining the standard Unix utility programs. In particular, the program is used to produce searches in an input file, selects the lines that match a given regular expression, and sorts the matched lines in reverse order.

The program takes in three arguments:
$ ./grepsort PATTERN INPUTFILE OUTPUTFILE

Where PATTERN is a simple string or a simple regular expression. INPUTFILE is the name of the input file, and OUTPUTFILE is the name of the output file for the result.
You are not expected to implement the specific functions, such as matching a regular expression or sorting the lines in reverse order. Rather, you should be using existing Unix utility programs:

grep is a utility program which is a file pattern searcher. It takes one command-line argument (the PATTERN string in our case) and search for matches line by line from the standard input (or the file specified as the second command-line argument). When a match is found (a line that matches with the PATTERN string), the program prints the line to the standard output. The utility program outputs all matched lines.

sort is a utility program that sorts the text file by lines. By default, it reads from the standard input and generates the sorted text to the standard output. Use -r option to sort in reverse order. 

Program uses fork and exec system calls to run the utility programs. You should also use pipe and dup/dup2 system calls to connect them, so that, for example, the standard output of the grep program is "piped" to the standard input of the sort program.

Implementation
You are advised to follow the steps below in your implementation:
1)Open input file for read
2) Open output file for write
3) Create a pipe 
4) Fork

5) In the parent process:
a) Use dup/dup2 to make opened input file as standard input
b) Use dup/dup2 to make the write end of the pipe as standard output
c) Close the open files not used (such as the output file and the read end of the pipe)
d) Use exec to run grep PATTERN

6) In the child process:
a) Use dup/dup2 to make the read end of the pipe as standard input
b) Use dup/dup2 to make opened output file as standard output
c) Close unused open files
d) Use exec to run sort -r 
