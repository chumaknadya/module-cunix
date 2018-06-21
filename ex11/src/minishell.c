#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "hash.h"
#include "builtins.h"
#define LSH_RL_BUFSIZE 1024

int ls_command(char *input) {
    pid_t	pid;
    write(1,"\n",1);
	  pid = fork();
    if (pid == 0) {
        if (strcmp(input,"") == 0) {
             execl("/usr/bin/ls","ls");
        }
        execl("/usr/bin/ls","ls",input);
    } else if (pid < 0) {
		    fprintf(stderr, "Fork failed");
		    return 1;
	  } else {
        wait(NULL);
    }
    return 0;
}

int run_command(hashtable_t *ht, char *str_ptr,char *input) {
   if (strcmp("env", str_ptr) == 0){
     env(ht, input);
     write(1,"\n",1);
   } else if (strcmp("export", str_ptr) == 0) {
     export(ht, input);
     write(1,"\n",1);
   } else if (strcmp("exit", str_ptr) == 0)
     return 1;
   else if(strcmp("echo", str_ptr) == 0){
     echo(ht, input);
     write(1,"\n",1);
   } else if(strcmp("ls", str_ptr) == 0) {
     ls_command(input);
   }
   else if(strlen(str_ptr) > 0)
     printf("Command not found: %s\n", str_ptr);
   return 0;
}

int exec_command(hashtable_t *ht,char *input) {
  char *str, *str_ptr;

  str = malloc(32 * sizeof(char));
  str_ptr = str;

  while(*input != '\0') {
    if(*input == ' ') {
      *str = '\0';
      input++;
      break;
    }
    *str++ = *input++;
  }
  *str = '\0';
  return run_command(ht,str_ptr,input);
}

int process_vars(hashtable_t *ht, char **input) {
  char *iptr, *str_ptr, *str;
  char *word, *word_ptr;

  str_ptr = malloc(512 * sizeof(char));
  word_ptr = malloc(32 * sizeof(char));
  iptr = *input;
  str = str_ptr;

  while(*iptr != '\0') {
    if (*iptr == '$') {
      iptr++;
      word = word_ptr;
      while(*iptr != ' ' && *iptr != '\0')
         *word++ = *iptr++;
      *word = '\0';
      word = (char *)hash_get(ht, word_ptr);
      if(!word) {
        continue;
      }
      while(*word != '\0')
        *str++ = *word++;

      continue;
    }
    *str++ = *iptr++;
  }
  *str = '\0';

  *input = str_ptr;

  return 0;
}

int	get_input(hashtable_t *ht) {
	int		ret;
	char	buf;
	int		i;
	int		count;
  char *input = (char *)malloc(512 * sizeof(char));
  char *ptr = input;
	count = 1;
	i = 0;
	while ((ret = read(0, &buf, 1)) && buf != '\n') {
		*(input + i++)= buf;
		count++;
	}
	*(input + i) = '\0';
	if (!ret) {
    free(ptr);
    return 1;
	}
  if(process_vars(ht, &ptr) != 0) {
    write(1, "error", 5);
  }
  return exec_command(ht,ptr);
}

void display_msg(void) {
  write(1,"$_>", 3);
}

void signal_handler(int signo) {
	if (signo == SIGINT) {
		puts("");
  	display_msg();
		signal(SIGINT, signal_handler);
	}
}

int main(int argc, char **argv, char **envv) {
  int		ret;
  char	**commands;
  hashtable_t *ht;
  int status;
  ht = hash_create(64);
  do {
    display_msg();
    signal(SIGINT, signal_handler);
    status = get_input(ht);
  } while (status == 0);
  write(1, "\n", 1);
  return 0;
}
