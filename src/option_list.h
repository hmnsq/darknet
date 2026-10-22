#اگر_غیر_تعریف_شدہ OPTION_LIST_H
#تعریف_کرو OPTION_LIST_H
#شامل "list.h"

قسمتعریف ساخت{
    حرف *کلید;
    حرف *val;
    صحیح مستعمل;
} kvp;


صحیح read_option(حرف *s, list *options);
خالی option_داخل_کرو(list *l, حرف *کلید, حرف *val);
حرف *option_ڈھونڈو(list *l, حرف *کلید);
اعشاری option_ڈھونڈو_float(list *l, حرف *کلید, اعشاری def);
اعشاری option_ڈھونڈو_float_quiet(list *l, حرف *کلید, اعشاری def);
خالی option_غیر_مستعمل(list *l);

#اگر_ختم
