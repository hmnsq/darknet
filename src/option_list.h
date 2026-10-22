#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#include "list.h"

typedef struct{
    char *सुसि;
    char *val;
    int used;
} kvp;


int read_option(char *s, list *options);
void option_इसिंगित(list *l, char *सुसि, char *val);
char *option_find(list *l, char *सुसि);
float option_find_float(list *l, char *सुसि, float def);
float option_find_float_quiet(list *l, char *सुसि, float def);
void option_unused(list *l);

#endif
