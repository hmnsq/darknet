#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPTION_LIST_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPTION_LIST_H
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ{
    ਅੱਖਰ *ਕੁੰਜੀ;
    ਅੱਖਰ *val;
    ਪੂਰਨ ਵਰਤਿਆ;
} kvp;


ਪੂਰਨ read_option(ਅੱਖਰ *s, list *options);
ਖਾਲੀ option_ਪਾਓ(list *l, ਅੱਖਰ *ਕੁੰਜੀ, ਅੱਖਰ *val);
ਅੱਖਰ *option_ਲੱਭੋ(list *l, ਅੱਖਰ *ਕੁੰਜੀ);
ਦਸ਼ਮਲਵ option_ਲੱਭੋ_float(list *l, ਅੱਖਰ *ਕੁੰਜੀ, ਦਸ਼ਮਲਵ def);
ਦਸ਼ਮਲਵ option_ਲੱਭੋ_float_quiet(list *l, ਅੱਖਰ *ਕੁੰਜੀ, ਦਸ਼ਮਲਵ def);
ਖਾਲੀ option_ਅਣਵਰਤਿਆ(list *l);

#ਜੇ_ਅੰਤ
