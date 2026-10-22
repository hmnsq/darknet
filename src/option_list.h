#यदि_अपरिभाषितम् OPTION_LIST_H
#परिभाषय OPTION_LIST_H
#अन्तर्भाव "list.h"

प्रकारनिर्देश संरचना{
    वर्ण *कुञ्चिका;
    वर्ण *val;
    पूर्णाङ्क प्रयुक्तम्;
} kvp;


पूर्णाङ्क read_option(वर्ण *s, list *options);
शून्य option_निवेशय(list *l, वर्ण *कुञ्चिका, वर्ण *val);
वर्ण *option_find(list *l, वर्ण *कुञ्चिका);
भिन्न option_find_float(list *l, वर्ण *कुञ्चिका, भिन्न def);
भिन्न option_find_float_quiet(list *l, वर्ण *कुञ्चिका, भिन्न def);
शून्य option_अप्रयुक्तम्(list *l);

#यदि_अन्तः
