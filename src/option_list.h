#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *کونچي;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_سيسيڤ(list *l, حرف *کونچي, حرف *val);
حرف *option_find(list *l, حرف *کونچي);
عائم option_find_float(list *l, حرف *کونچي, عائم def);
عائم option_find_float_quiet(list *l, حرف *کونچي, عائم def);
فراغ option_unused(list *l);

#endif
