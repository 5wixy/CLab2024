//
// Created by gyank on 03/07/2024.
//

#ifndef CLAB_MACRO_H
#define CLAB_MACRO_H


typedef struct Macro {
    char *name;
    char *content;
    struct Macro *next;
} Macro;

Macro *createMacro(char *name, char *content);
void addMacro(Macro **head, char *name, char *content);
void printMacros(Macro *head);
void freeMacros(Macro *head);


#endif //CLAB_MACRO_H
