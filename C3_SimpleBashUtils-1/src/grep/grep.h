#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <locale.h>  //Это нужно для корреткной работы грепа с русскими буквами - setlocale(LC_ALL, "");
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_LENGTH 500
#define STRING_LENGTH 4096

typedef struct arguments {
  int e, i, v, c, l, n, h, s;
} arguments;

// void print_arguments(arguments args);

int parser(int argc, char *argv[], arguments *arg, char *e_template);
int read(int argc, char *argv[], arguments *arg, char *e_template);
int switch_arg(int argc, char *argv[], arguments *arg,
               /* int current_arg,*/ char *e_template);
void output(arguments *arg, FILE *file, regex_t *regul, char *file_name,
            int many_files);