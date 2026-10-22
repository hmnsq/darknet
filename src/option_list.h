#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#شامل "list.h"

تعریفنوع ساختار{
    نویسه *کلید;
    نویسه *val;
    صحیح used;
} kvp;


صحیح read_option(نویسه *s, list *options);
تهی option_درج(list *l, نویسه *کلید, نویسه *val);
نویسه *option_find(list *l, نویسه *کلید);
اعشاری option_find_float(list *l, نویسه *کلید, اعشاری def);
اعشاری option_find_float_quiet(list *l, نویسه *کلید, اعشاری def);
تهی option_unused(list *l);

#endif
