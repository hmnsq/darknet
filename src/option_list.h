#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#অন্তর্ভাব "list.h"

প্রকারলেখ কাঠামো{
    অক্ষর *চাবি;
    অক্ষর *val;
    পূর্ণসংখ্যা used;
} kvp;


পূর্ণসংখ্যা read_option(অক্ষর *s, list *options);
শূন্য option_ঢোকাও(list *l, অক্ষর *চাবি, অক্ষর *val);
অক্ষর *option_find(list *l, অক্ষর *চাবি);
দশমিক option_find_float(list *l, অক্ষর *চাবি, দশমিক def);
দশমিক option_find_float_quiet(list *l, অক্ষর *চাবি, দশমিক def);
শূন্য option_unused(list *l);

#endif
