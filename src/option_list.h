#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#अन्तर्भाव "list.h"

प्रकारव्याख्या संरचना{
    अक्षर *किल्ली;
    अक्षर *val;
    पूर्णांक used;
} kvp;


पूर्णांक read_option(अक्षर *s, list *options);
शून्य option_घाला(list *l, अक्षर *किल्ली, अक्षर *val);
अक्षर *option_find(list *l, अक्षर *किल्ली);
दशांश option_find_float(list *l, अक्षर *किल्ली, दशांश def);
दशांश option_find_float_quiet(list *l, अक्षर *किल्ली, दशांश def);
शून्य option_unused(list *l);

#endif
