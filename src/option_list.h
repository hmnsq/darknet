#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#ଅନ୍ତର୍ଭାଵ "list.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା{
    ଅକ୍ଷର *ଚାବି;
    ଅକ୍ଷର *val;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା used;
} kvp;


ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_option(ଅକ୍ଷର *s, list *options);
ଶୂନ୍ୟ option_ଭର୍ତ୍ତି(list *l, ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର *val);
ଅକ୍ଷର *option_find(list *l, ଅକ୍ଷର *ଚାବି);
ଦଶମିକ option_find_float(list *l, ଅକ୍ଷର *ଚାବି, ଦଶମିକ def);
ଦଶମିକ option_find_float_quiet(list *l, ଅକ୍ଷର *ଚାବି, ଦଶମିକ def);
ଶୂନ୍ୟ option_unused(list *l);

#endif
