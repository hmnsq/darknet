#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#অন্তর্ভাব "list.h"

প্রকারনির্দেশ সংরচনা{
    বর্ণ *চাবি;
    বর্ণ *val;
    পূর্ণাঙ্ক used;
} kvp;


পূর্ণাঙ্ক read_option(বর্ণ *s, list *options);
শূন্য option_ঢোকাও(list *l, বর্ণ *চাবি, বর্ণ *val);
বর্ণ *option_find(list *l, বর্ণ *চাবি);
ভিন্ন option_find_float(list *l, বর্ণ *চাবি, ভিন্ন def);
ভিন্ন option_find_float_quiet(list *l, বর্ণ *চাবি, ভিন্ন def);
শূন্য option_unused(list *l);

#endif
