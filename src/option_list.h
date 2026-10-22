#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#അന്തര്ഭാവ "list.h"

തരനിർവചനം ഘടന{
    അക്ഷരം *താക്കോൽ;
    അക്ഷരം *val;
    പൂർണ്ണസംഖ്യ used;
} kvp;


പൂർണ്ണസംഖ്യ read_option(അക്ഷരം *s, list *options);
ശൂന്യം option_തിരുകുക(list *l, അക്ഷരം *താക്കോൽ, അക്ഷരം *val);
അക്ഷരം *option_find(list *l, അക്ഷരം *താക്കോൽ);
ദശാംശം option_find_float(list *l, അക്ഷരം *താക്കോൽ, ദശാംശം def);
ദശാംശം option_find_float_quiet(list *l, അക്ഷരം *താക്കോൽ, ദശാംശം def);
ശൂന്യം option_unused(list *l);

#endif
