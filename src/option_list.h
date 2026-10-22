#ifndef OPTION_LIST_H
#define OPTION_LIST_H
#अन्तर्भाव "list.h"

प्रकारनिर्देश संरचना{
    वर्ण *साँचो;
    वर्ण *val;
    पूर्णाङ्क used;
} kvp;


पूर्णाङ्क read_option(वर्ण *s, list *options);
शून्य option_घुसाउ(list *l, वर्ण *साँचो, वर्ण *val);
वर्ण *option_find(list *l, वर्ण *साँचो);
भिन्न option_find_float(list *l, वर्ण *साँचो, भिन्न def);
भिन्न option_find_float_quiet(list *l, वर्ण *साँचो, भिन्न def);
शून्य option_unused(list *l);

#endif
