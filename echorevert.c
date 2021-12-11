/* 

This program reverses a string, based on whatever the user inputs
whether it is numbers, lets or a mixture of both.
This is done by reading from a file, then inverting it, reversing
and then writing it back.

*/

//Bringing in necessary Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// Defining buffer size
#define BUFSIZ 1024

// Method that reverses a string, takes in buffer 
void revert(char* buf) {

  // Creating of variables
  int i = 0, j = strlen(buf)-1;

    // While loop for buffer size 
    while(i < j) { 
    char c = buf[j];
    buf[j--] = buf[i];
    buf[i++] = c;
  }
}

// Main function
int main(){
  // Creates a processes ID called pid
  pid_t pid;

  // Creates 2 pipes, p2c[2] means parent-to-child , c2p[2] is the child-to-parent.
  int p2c[2], c2p[2], n; 
  char buf[BUFSIZ];

// If statement created to catch whether the parent or child variables are empty
  if(pipe(p2c) < 0 || pipe(c2p) < 0) {   
    perror("pipe failed");            
    return 1; 
  }
   
  //Calls the fork method		   
  pid = fork(); 
  if(pid < 0) { 

    /* Outputs to the an error occurred */
    perror("fork failed");
    exit(-1);
  } 

/* Child process will read data from a file, invert, reverse (the string) and write back to parent process*/
else if(pid == 0){ 

    // Closes the write part from the parent to child pipe
    close(p2c[1]); 

    // Closes the read part from the child to parent pipe
    close(c2p[0]);

    //Forever loop, unless there is no more input
    for(;;) { 
      
      // Takes input from the parent pipe,
      n = read(p2c[0], buf, BUFSIZ); 
      if(n < 0) exit(-1);
      else if(n == 0) continue;

      //Inverts the input
      revert(buf);

      // The result is then written back through the descriptor side of the c2p[1]
      n = write(c2p[1], buf, strlen(buf)+1); 
      if(n < 0) exit(-1);
    }
  } 

/* Parent process will read data from the file, give it to the child process and 
   the child will reverse the string, sends it back to the parent which will read it and output result */
else { 
    
    // Closes the read part from the parent to child pipe
    close(p2c[0]);

    // Closes the from the write part from the child to parent pipe
    close(c2p[1]);

    // Parent process gets information from standard in (keyboard)
    while(fgets(buf, BUFSIZ, stdin)) { 
      int len = strlen(buf);

      /* remove the newline */
      if(len > 0 && buf[len-1] == '\n') buf[--len] = 0; 

      /* terminates when empty line is encountered */
      if(len <= 0) break; 
      /*printf("%s", buf);*/

      //Writes through the descriptor from parent to child, information from keyboard
      n = write(p2c[1], buf, len+1); 
      if(n < 0) exit(-1);

      //Reads from the child to parent pipe 
      n = read(c2p[0], buf, BUFSIZ);  
       
      //Reads from the reading end
      if(n < 0) exit(-1); 

      //Outputs the result
      printf("%s\n", buf); //Outputs the result
    }

    //Command used to terminate program, sends a signal
    kill(pid, SIGKILL); 
  }
  return 0;
}
