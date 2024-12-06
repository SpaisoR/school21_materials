#include "grep.h"

int main(int argc, char **argv) {
  arguments arg = {0};
  char e_template[4096] = {'\0'};
  int flag = parser(argc, argv, &arg, e_template);
  // print_arguments(arg);
  if (flag) read(argc, argv, &arg, e_template);
  return 0;
}
/* // Показывает какие флаги включены
void print_arguments(arguments args) {
    printf("e: %d\n", args.e);
    printf("i: %d\n", args.i);
    printf("v: %d\n", args.v);
    printf("c: %d\n", args.c);
    printf("l: %d\n", args.l);
    printf("n: %d\n", args.n);
    printf("h: %d\n", args.h);
    printf("s: %d\n", args.s);
}*/

int parser(int argc, char *argv[], arguments *arg, char *e_template) {
  // int current_arg;
  int res = 1;
  // opterr = 0; // переменная getopt-а (отключает все сообщения об ошибках).
  // while ((current_arg = getopt(argc, argv, "e:ivclnhs")) != -1) {
  switch_arg(argc, argv, arg, /* current_arg,*/ e_template);
  //}
  if (argc == 1 && arg->s != 1) {
    fprintf(stderr, "Input file not provided\n");
    res = 0;
  }
  return res;
}

int switch_arg(int argc, char *argv[], arguments *arg,
               /* int current_arg,*/ char *e_template) {
  int current_arg;
  while ((current_arg = getopt(argc, argv, "e:ivclnhs")) != -1) {
    switch (current_arg) {
      case 'e':
        arg->e = 1;
        if (*e_template != '\0') strcat(e_template, "|");
        strcat(e_template, optarg);
        // printf("e_template = %s\n", e_template); //ЭТО УДАЛИТЬ
        break;
      case 'i':
        arg->i = 1;
        break;
      case 'v':
        arg->v = 1;
        break;
      case 'c':
        if (!arg->l) {
          arg->c = 1;
        }
        break;
      case 'l':
        arg->l = 1;
        arg->h = 0;
        arg->c = 0;
        arg->n = 0;
        break;
      case 'n':
        if (!arg->l || !arg->c) {
          arg->n = 1;
        }
        break;
      case 'h':
        if (!arg->l || !arg->c) {
          arg->h = 1;
        }
        break;
      case 's':
        arg->s = 1;
        break;
      case '?':
        if (arg->s == 0) {
          fprintf(stderr, "Unknown flag %c\n", optopt);
        }
        break;
      default:
        break;
    }
  }
  if (arg->e == 0 && optind < argc) {
    if (*e_template != '\0') strcat(e_template, "|");
    strcat(e_template, argv[optind++]);
    /* здесь optind++ нужен, чтобы первый нефлаг
    был считан как шаблон, и переключить
    индекс уже на следующий нефлаг, то есть файл */
    // printf("e_template = %s\n", e_template); //ЭТО УДАЛИТЬ
  }
  return 0;
}

int read(int argc, char *argv[], arguments *arg, char *e_template) {
  int many_files = argc - optind > 1 ? 1 : 0;  // тернарный оператор
  regex_t regul;  // объявляем переменную regul типа regex_t
  int reg_flags = arg->i == 0 ? REG_EXTENDED : REG_EXTENDED | REG_ICASE;
  /*int reg_flags = REG_EXTENDED;
  printf("reg_flags = %d\n", reg_flags);  // УДАЛИТЬ
  if (arg->i) {
    reg_flags |= REG_ICASE;
    printf("reg_flags = %d\n", reg_flags);  // УДАЛИТЬ
  }*/
  /*
  REG_EXTENDED - расширенный синтаксис регулярныйх выражений
  то есть не нужно экранировать спец символы обратным слешем
  REG_ICASE - игнорируем регистр
  "|" - побитовое ИЛИ.
  int reg_flags = REG_EXTENDED = 0001
  int reg_flags = REG_ICASE = 0010
  int reg_flags = REG_EXTENDED | REG_ICASE = 0011
  */
  setlocale(LC_ALL, "");  // Это нужно для корреткной работы с русскими буквами

  int result_comp = regcomp(&regul, e_template, reg_flags);
  // printf("result_comp = %d\n", result_comp); // УДАЛИТЬ
  if (result_comp != 0) {
    if (arg->s == 0) printf("Regex compilation error.\n");
    regfree(&regul);  // освобождаем память
    return 1;
  }
  /*else {                                      // УДАЛИТЬ
    printf("Regex compiled successfully.\n"); // УДАЛИТЬ
  }                                           // УДАЛИТЬ
  printf("many_files = %d\n", many_files); */   // УДАЛИТЬ
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      // printf("optind = %s\n", argv[i]); //ЭТО УДАЛИТЬ
      output(arg, file, &regul, argv[i], many_files);
      fclose(file);
    } else {
      if (arg->s == 0) fprintf(stderr, "Error opening file '%s'\n", argv[i]);
    }
  }
  regfree(&regul);
  return 0;
}

void output(arguments *arg, FILE *file, regex_t *regul, char *file_name,
            int many_files) {
  char line[PATTERN_LENGTH];
  int line_number = 0, was_print = 0, match_count = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    line_number++;
    int match_found = regexec(regul, line, 0, NULL, 0);
    // printf("regexec result: %d, line: %s\n", match_found, line);
    // printf("match_found = %d\n", match_found); // УДАЛИТЬ
    if (arg->v == 1) {
      match_found = !match_found;
    }
    if (match_found == 0) {
      match_count++;
      if (arg->l == 1 && was_print == 0) {
        was_print = 1;
        // printf("%s\n", file_name);
      }
      /*
      if (arg->c == 1 && was_print == 0) {
        was_print = 1;
        printf("%d\n", match_count);
      }*/
      if (arg->l == 0 && arg->c == 0) {  // эти флаги отменяют остальные
        if (many_files == 1 && arg->h == 0)
          printf("%s:", file_name);  // если файлов несколько, тогда нужно
                                     // вывыести его название
        if (arg->n == 1) printf("%d:", line_number);
        printf("%s", line);
        if (strchr(line, '\n') == NULL) printf("\n");  // узнать что за функция
      }
    }
  }
  if (arg->c == 1 && was_print == 0) {
    if (many_files && arg->h == 0) printf("%s:", file_name);
    printf("%d\n", match_count);
  }
  if (was_print == 1) {
    printf("%s\n", file_name);
  }
  /*

  if (check->c == 1 && print_name == 0) {
    if (many_files) printf("%s:", file_name);
    printf("%d\n", match_count);
  }
  if (print_name == 1) {
    printf("%s\n", file_name);
  }*/
}
