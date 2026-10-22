#جے_بنا_تعریف OPTION_LIST_H
#تعریف_کرو OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *کلید;
    حرف *val;
    صحيح ورتیا;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_داخل_کرو(list *l, حرف *کلید, حرف *val);
حرف *option_لبھو(list *l, حرف *کلید);
عائم option_لبھو_float(list *l, حرف *کلید, عائم def);
عائم option_لبھو_float_quiet(list *l, حرف *کلید, عائم def);
فراغ option_بن_ورتیا(list *l);

#جے_انت
