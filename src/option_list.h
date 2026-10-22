#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *مفتاح;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_أدرج(list *l, حرف *مفتاح, حرف *val);
حرف *option_find(list *l, حرف *مفتاح);
عائم option_find_float(list *l, حرف *مفتاح, عائم def);
عائم option_find_float_quiet(list *l, حرف *مفتاح, عائم def);
فراغ option_unused(list *l);

#endif
