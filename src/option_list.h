#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#অন্তর্ভাৱ "list.h"

প্রকারনির্দেশ সংরচনা{
    ৱর্ণ *চাবি;
    ৱর্ণ *val;
    পূর্ণাঙ্ক used;
} kvp;


পূর্ণাঙ্ক read_option(ৱর্ণ *s, list *options);
শূন্য option_ঢোকাও(list *l, ৱর্ণ *চাবি, ৱর্ণ *val);
ৱর্ণ *option_find(list *l, ৱর্ণ *চাবি);
ভিন্ন option_find_float(list *l, ৱর্ণ *চাবি, ভিন্ন def);
ভিন্ন option_find_float_quiet(list *l, ৱর্ণ *চাবি, ভিন্ন def);
শূন্য option_unused(list *l);

#endif
