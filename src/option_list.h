#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *ئاچقۇچ;
    حرف *val;
    صحيح used;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_قىستۇرۇش(list *l, حرف *ئاچقۇچ, حرف *val);
حرف *option_find(list *l, حرف *ئاچقۇچ);
عائم option_find_float(list *l, حرف *ئاچقۇچ, عائم def);
عائم option_find_float_quiet(list *l, حرف *ئاچقۇچ, عائم def);
فراغ option_unused(list *l);

#endif
