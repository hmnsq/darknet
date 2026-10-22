#اگر_تعریف_نشده OPTION_LIST_H
#تعریف_کن OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *کلید;
    حرف *val;
    صحيح استفاده_شده;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_درج(list *l, حرف *کلید, حرف *val);
حرف *option_پیدا_کن(list *l, حرف *کلید);
عائم option_پیدا_کن_float(list *l, حرف *کلید, عائم def);
عائم option_پیدا_کن_float_quiet(list *l, حرف *کلید, عائم def);
فراغ option_استفادهنشده(list *l);

#پایان_اگر
