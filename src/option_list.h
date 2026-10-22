#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *key;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_insert(list *l, حرف *key, حرف *val);
حرف *option_find(list *l, حرف *key);
عائم option_find_float(list *l, حرف *key, عائم def);
عائم option_find_float_quiet(list *l, حرف *key, عائم def);
فراغ option_unused(list *l);

#endif
