#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/macro.h"

Macro *createMacro(char *name, char *content) {
    Macro *newMacro = (Macro *)malloc(sizeof(Macro));
    newMacro->name = strdup(name); // Ensure to duplicate the string
    newMacro->content = strdup(content); // Ensure to duplicate the string
    newMacro->next = NULL;
    return newMacro;
}

void addMacro(Macro **head, char *name, char *content) {
    Macro *newMacro = createMacro(name, content);
    newMacro->next = *head;
    *head = newMacro;
}

void printMacros(Macro *head) {
    Macro *current = head;
    while (current != NULL) {
        printf("Macro Name: %s\n", current->name);
        printf("Content:\n%s\n", current->content);
        current = current->next;
    }
}

void freeMacros(Macro *head) {
    Macro *current = head;
    Macro *next;
    while (current != NULL) {
        next = current->next;
        free(current->name);
        free(current->content);
        free(current);
        current = next;
    }
}