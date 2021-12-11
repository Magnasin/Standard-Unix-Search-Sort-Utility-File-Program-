#include "grepper.h"

int main(int argc, char **argv)
{
    // ensure there are three arguments (total 4)
    if(argc != 4)
    {
        printf("Usage : %s PATTERN INPUTFILE OUTPUTFILE\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    // else we have the right number of arguments
    int IN, OUT, PID;

    // open input file for reading
    IN = open(argv[2], O_RDONLY);

    // open output file for writing. 
    // flags for writing, create and truncate if exists
    OUT = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666);

    // create pipe
    int pipefd[2];
    pipe(pipefd); // sets up pipe

    // pipefd[0] = read end of grep->sort pipe (read by sort)
    // pipefd[1] = write end of grep->sort pipe (written by grep)
    
    char *grep_args[] = {"grep", argv[1], NULL};
    char *sort_args[] = {"sort", "-r", NULL};
    
    // fork to run the command
    PID = fork();

    if(PID == 0)
    {
        // child : handles sort -r
        
        // replace stdin with input part of pipe : written by grep
        dup2(pipefd[0], 0);

        // replace stdout with output file
        dup2(OUT, 1);

        // close unused end of pipe : write end (used by grep)
        close(pipefd[1]);

        // close input file (read by grep)
        close(IN);

        // execute sort
        execvp("sort", sort_args);
    }
    else
    {
        // parent : handles grep

        // replace stdin (0) with input file
        dup2(IN, 0);

        // replace stdout with write end of pipe
        dup2(pipefd[1], 1);

        // close unused half of the pipe : read end (will be read by sort)
        close(pipefd[0]);

        // close output file. will be used by sort
        close(OUT);

        // run exec with ability to search path from environment variables
        execvp("grep", grep_args);
    }
}