#که_نه_تعریف_شوی OPTION_LIST_H
#تعریف_کړه OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *کلی;
    حرف *val;
    صحيح کارول_شوی;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_ورننباسه(list *l, حرف *کلی, حرف *val);
حرف *option_پیدا_کړه(list *l, حرف *کلی);
عائم option_پیدا_کړه_float(list *l, حرف *کلی, عائم def);
عائم option_پیدا_کړه_float_quiet(list *l, حرف *کلی, عائم def);
فراغ option_ناکارول_شوی(list *l);

#د_که_پای
