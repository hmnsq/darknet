#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *کلید;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_داخل_کرو(list *l, حرف *کلید, حرف *val);
حرف *option_find(list *l, حرف *کلید);
عائم option_find_float(list *l, حرف *کلید, عائم def);
عائم option_find_float_quiet(list *l, حرف *کلید, عائم def);
فراغ option_unused(list *l);

#endif
