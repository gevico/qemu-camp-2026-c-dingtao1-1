#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

static char arg_storage[MAX_ARGS][MAX_INPUT];

// ======================
// 自定义命令系统
// ======================

typedef int (*cmd_func_0_t)(void);
typedef int (*cmd_func_1_t)(const char*);
typedef int (*cmd_func_2_t)(const char*, const char*);

typedef struct {
    const char *name;
    int is_arg_required;
    union {
        cmd_func_0_t func_0;
        cmd_func_1_t func_1;
        cmd_func_2_t func_2;
    } func;
} Command;

Command commands[] = {
    {"myfile", 1, .func.func_1 = __cmd_myfile},
    {"mysed",  2, .func.func_2 = __cmd_mysed},
    {"mytrans", 1, .func.func_1 = __cmd_mytrans},
    {"mywc", 1, .func.func_1 = __cmd_mywc},
    {NULL, 0, .func.func_0 = NULL}
};

// ======================
// 内置命令
// ======================

void execute_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "mybash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mybash");
    }
  }
}

void execute_exit(void) { exit(0); }

int is_builtin_command(char **args) {
  if (args[0] == NULL)
    return 0;

  if (strcmp(args[0], "cd") == 0) {
    execute_cd(args);
    return 1;
  }
  if (strcmp(args[0], "exit") == 0) {
    execute_exit();
    return 1;
  }
  return 0;
}

int parse_input(char *input, char **args) {
  int i = 0;
  char *p = input;

  while (*p != '\0' && i < MAX_ARGS - 1) {
    while (*p == ' ' || *p == '\t')
      p++;
    if (*p == '\0')
      break;

    if (*p == '"') {
      p++;
      int j = 0;
      while (*p != '\0' && *p != '"' && j < MAX_INPUT - 1)
        arg_storage[i][j++] = *p++;
      arg_storage[i][j] = '\0';
      if (*p == '"')
        p++;
      args[i] = arg_storage[i];
      i++;
    } else {
      int j = 0;
      while (*p != '\0' && *p != ' ' && *p != '\t' && j < MAX_INPUT - 1)
        arg_storage[i][j++] = *p++;
      arg_storage[i][j] = '\0';
      args[i] = arg_storage[i];
      i++;
    }
  }

  args[i] = NULL;
  return i;
}

// ======================
// 主函数
// ======================

int main(int argc, char *argv[]) {
  char input[MAX_INPUT];
  char *args[MAX_ARGS];

  if (argc > 1) {
    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
      printf("mybash: cannot open file: %s\n", filename);
      return 1;
    }

    printf("mybash: reading commands from file '%s'\n", filename);

    while (fgets(input, sizeof(input), file)) {
      input[strcspn(input, "\n")] = '\0';

      int argc_parsed = parse_input(input, args);

      if (argc_parsed == 0) {
        continue;
      }

      if (is_builtin_command(args)) {
        continue;
      }

      const char *cmd_name = args[0];
      const char *cmd_arg1 = (argc_parsed >= 2) ? args[1] : NULL;
      const char *cmd_arg2 = (argc_parsed >= 3) ? args[2] : NULL;

      int found = 0;
      for (Command *cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd_name, cmd->name) == 0) {
          found = 1;
          if (cmd->is_arg_required == 0) {
            cmd->func.func_0();
          } else if (cmd->is_arg_required == 1) {
            cmd->func.func_1(cmd_arg1);
          } else if (cmd->is_arg_required == 2) {
            cmd->func.func_2(cmd_arg1, cmd_arg2);
          }
          break;
        }
      }

      if (!found) {
        fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
      }
    }

    fclose(file);
    return 0;
  }

  while (1) {
    printf("mybash$ ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      printf("\n");
      break;
    }

    input[strcspn(input, "\n")] = '\0';

    int ac = parse_input(input, args);

    if (ac == 0) {
      continue;
    }

    if (is_builtin_command(args)) {
      continue;
    }

    const char *cmd_name = args[0];
    const char *cmd_arg1 = (ac >= 2) ? args[1] : NULL;
    const char *cmd_arg2 = (ac >= 3) ? args[2] : NULL;

    int found = 0;
    for (Command *cmd = commands; cmd->name != NULL; cmd++) {
      if (strcmp(cmd_name, cmd->name) == 0) {
        found = 1;
        if (cmd->is_arg_required == 0) {
          cmd->func.func_0();
        } else if (cmd->is_arg_required == 1) {
          cmd->func.func_1(cmd_arg1);
        } else if (cmd->is_arg_required == 2) {
          cmd->func.func_2(cmd_arg1, cmd_arg2);
        }
        break;
      }
    }

    if (!found) {
      fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
    }
  }

  return 0;
}
