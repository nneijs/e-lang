#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100

typedef struct {
    char name[50];
    int value;
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

int find_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void set_variable(const char *name, int value) {
    int index = find_variable(name);
    if (index == -1) {
        strncpy(variables[var_count].name, name, 50);
        variables[var_count].value = value;
        var_count++;
    } else {
        variables[index].value = value;
    }
}

int get_variable(const char *name) {
    int index = find_variable(name);
    return (index == -1) ? 0 : variables[index].value;
}

void interpret_line(char *line) {
    char command[10];
    sscanf(line, "%s", command);

    if (strcmp(command, "print") == 0) {
        char arg[50];
        sscanf(line + strlen(command), "%s", arg);
        if (arg[0] == '"') {
            printf("%s\n", arg + 1);
        } else {
            printf("%d\n", get_variable(arg));
        }
    } else if (strcmp(command, "set") == 0) {
        char var_name[50];
        int value;
        sscanf(line + strlen(command), "%s %d", var_name, &value);
        set_variable(var_name, value);
    } else if (strcmp(command, "add") == 0) {
        char target[50], op1[50], op2[50];
        sscanf(line + strlen(command), "%s %s %s", target, op1, op2);
        
        int operand1 = find_variable(op1) != -1 ? get_variable(op1) : atoi(op1);
        int operand2 = find_variable(op2) != -1 ? get_variable(op2) : atoi(op2);
        
        set_variable(target, operand1 + operand2);
    } else {
        printf("Unknown command: %s\n", command);
    }
}

void interpret_script(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        interpret_line(line);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: run <filename>.e\n");
        return 1;
    }

    const char *filename = argv[1];
    if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 5, ".e") != 0) {
        fprintf(stderr, "Error: File extension should be .e\n");
        return 1;
    }

    interpret_script(filename);
    return 0;
}
