#جيڪڏهن_وصف_نه_ٿيل OPTION_LIST_H
#وصف_ڪريو OPTION_LIST_H
#ضمن "list.h"

تعريفنوع بنية{
    حرف *ڪنجي;
    حرف *val;
    صحيح استعمال_ٿيل;
} kvp;


صحيح read_option(حرف *s, list *options);
فراغ option_وجهو(list *l, حرف *ڪنجي, حرف *val);
حرف *option_ڳوليو(list *l, حرف *ڪنجي);
عائم option_ڳوليو_float(list *l, حرف *ڪنجي, عائم def);
عائم option_ڳوليو_float_quiet(list *l, حرف *ڪنجي, عائم def);
فراغ option_اڻ_استعمال_ٿيل(list *l);

#جيڪڏهن_پورو
