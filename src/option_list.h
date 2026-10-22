#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *مبودي;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_سك(list *l, حرف *مبودي, حرف *val);
حرف *option_find(list *l, حرف *مبودي);
عائم option_find_float(list *l, حرف *مبودي, عائم def);
عائم option_find_float_quiet(list *l, حرف *مبودي, عائم def);
فراغ option_unused(list *l);

#endif
