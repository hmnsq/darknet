#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#include "list.h"

typedef sa_nracanaa{
    var_na *key;
    var_na *val;
    puur_naa_nka used;
} kvp;


puur_naa_nka read_option(var_na *s, list *options);
void option_insert(list *l, var_na *key, var_na *val);
var_na *option_find(list *l, var_na *key);
float option_find_float(list *l, var_na *key, float def);
float option_find_float_quiet(list *l, var_na *key, float def);
void option_unused(list *l);

#endif
