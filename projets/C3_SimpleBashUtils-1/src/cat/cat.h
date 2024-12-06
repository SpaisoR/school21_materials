#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <stdio.h>

struct option long_arguments[] = {{"number", 0, 0, 'n'},
                                  {"number-nonblank", 0, 0, 'b'},
                                  {"squeeze-blank", 0, 0, 's'},
                                  {0, 0, 0, 0}};

typedef struct arguments {
  int b, n, s, E, e, T, t, v;
} arguments;

int parser(int argc, char *argv[], arguments *arg);
void read(int argc, char *argv[], arguments arg);
int switch_arg(arguments *arg, int current_arg);
void output(arguments arg, FILE *file, int *line, int *empty_line,
            int *ch_line);