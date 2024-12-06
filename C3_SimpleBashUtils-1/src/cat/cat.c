#include "cat.h"

int main(int argc, char **argv) {
  arguments arg = {0};
  int flag = parser(argc, argv, &arg);
  if (flag) read(argc, argv, arg);
  return 0;
}

int parser(int argc, char *argv[], arguments *arg) {
  int current_arg, flag = 1;
  if (argc == 1) {
    fprintf(stderr, "Input file not provided\n");
    flag = 0;
  } else {
    while ((current_arg = getopt_long(argc, argv, "bnseEtTv", long_arguments,
                                      NULL)) != -1) {
      switch_arg(arg, current_arg);
    }
  }
  return flag;
}

int switch_arg(arguments *arg, int current_arg) {
  switch (current_arg) {
    case 'b':
      arg->b = 1;
      arg->n = 0;
      break;
    case 'n':
      if (!arg->b) {
        arg->n = 1;
      }
      break;
    case 's':
      arg->s = 1;
      break;
    case 'E':
      arg->E = 1;
      break;
    case 'e':
      arg->E = arg->v = 1;
      break;
    case 'T':
      arg->T = 1;
      break;
    case 't':
      arg->T = arg->v = 1;
      break;
    case 'v':
      arg->v = 1;
      break;
  }
  return 0;
}

void read(int argc, char *argv[], arguments arg) {
  int line = 1, empty_line = 0, ch_line = 0;
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      output(arg, file, &line, &empty_line, &ch_line);
      fclose(file);
    } else
      fprintf(stderr, "Error opening file '%s'\n", argv[i]);
  }
}

void output(arguments arg, FILE *file, int *line, int *empty_line,
            int *ch_line) {
  int ch;

  while ((ch = fgetc(file)) != EOF) {
    if (arg.s && ch == '\n' && *ch_line == 0) {
      (*empty_line)++;
      if (*empty_line > 1) {
        continue;
      }
    }
    if (*ch_line == 0 && (arg.n || (arg.b && ch != '\n'))) {
      printf("%6d\t", *line);
      (*line)++;
    }
    if (arg.T && ch == '\t') {
      printf("^");
      ch = 'I';
    }
    if (arg.v && ch != '\n' && ch != '\t') {
      while (ch > 127) {
        printf("M-");
        ch -= 128;
      }
      while (ch < 0) {
        printf("M-");
        ch += 128;
      }
      if (ch < 32) {
        printf("^");
        ch += 64;
      }
      if (ch == 127) {
        printf("^");
        ch = '?';
      }
    }
    if (arg.E && ch == '\n') {
      printf("$");
    }
    putchar(ch);
    *ch_line = ch == '\n' ? 0 : 1;
    if (*ch_line == 1) {
      *empty_line = 0;
    }
  }
}
