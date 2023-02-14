#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
  int pipe1[2], pipe2[2];
  pid_t pid1, pid2, pid3;

  if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    perror("pipe");
    exit(1);
  }

  pid1 = fork();
  if (pid1 == -1) {
    perror("fork");
    exit(1);
  }
  if (pid1 == 0) {
    //ls -la
    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);
    dup2(pipe1[1], STDOUT_FILENO);
    close(pipe1[1]);
    execlp("ls", "ls", "-la", (char *) NULL);
    perror("execlp");
    exit(1);
  }
  close(pipe1[1]);

  pid2 = fork();
  if (pid2 == -1) {
    perror("fork");
    exit(1);
  }
  if (pid2 == 0) {
    // tee alf1.txt
    close(pipe2[0]);
    dup2(pipe1[0], STDIN_FILENO);
    close(pipe1[0]);
    dup2(pipe2[1], STDOUT_FILENO);
    close(pipe2[1]);
    execlp("tee", "tee", "alf1.txt", (char *) NULL);
    perror("execlp");
    exit(1);
  }
  close(pipe1[0]);
  close(pipe2[1]);

  pid3 = fork();
  if (pid3 == -1) {
    perror("fork");
    exit(1);
  }
  if (pid3 == 0) {
    //sort -r
    dup2(pipe2[0], STDIN_FILENO);
    close(pipe2[0]);
    execlp("sort", "sort", "-r", (char *) NULL);
    perror("execlp");
    freopen("salida.txt", "w", stdout);
        freopen("salida.txt", "w", stdout);
        fclose(stdout);
    exit(1);
    
  }
  close(pipe2[0]);

  
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  waitpid(pid3, NULL, 0);

  return 0;
}