#ifndef UTILS_H
#define UTILS_H
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव "darknet.h"
#अन्तर्भाव "list.h"

#define TIME(a) \
    गर { \
    द्विगुण start = what_time_is_it_now(); \
    a; \
    म_लिखो("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } जबसम्म (0)

#define TWO_PI 6.2831853071795864769252866f

द्विगुण what_time_is_it_now();
शून्य shuffle(शून्य *arr, माप_प्रकार n, माप_प्रकार size);
शून्य sorta_shuffle(शून्य *arr, माप_प्रकार n, माप_प्रकार size, माप_प्रकार sections);
शून्य free_ptrs(शून्य **ptrs, पूर्णाङ्क n);
पूर्णाङ्क alphanum_to_int(वर्ण c);
वर्ण int_to_alphanum(पूर्णाङ्क i);
पूर्णाङ्क read_int(पूर्णाङ्क fd);
शून्य write_int(पूर्णाङ्क fd, पूर्णाङ्क n);
शून्य read_all(पूर्णाङ्क fd, वर्ण *buffer, माप_प्रकार bytes);
शून्य write_all(पूर्णाङ्क fd, वर्ण *buffer, माप_प्रकार bytes);
पूर्णाङ्क read_all_fail(पूर्णाङ्क fd, वर्ण *buffer, माप_प्रकार bytes);
पूर्णाङ्क write_all_fail(पूर्णाङ्क fd, वर्ण *buffer, माप_प्रकार bytes);
शून्य find_replace(वर्ण *str, वर्ण *orig, वर्ण *rep, वर्ण *output);
शून्य malloc_error();
शून्य file_error(वर्ण *s);
शून्य strip(वर्ण *s);
शून्य strip_char(वर्ण *s, वर्ण bad);
list *split_str(वर्ण *s, वर्ण delim);
वर्ण *fgetl(खाता *fp);
list *parse_csv_पङ्क्ति(वर्ण *पङ्क्ति);
वर्ण *copy_string(वर्ण *s);
पूर्णाङ्क गन्ती_fields(वर्ण *पङ्क्ति);
भिन्न *parse_fields(वर्ण *पङ्क्ति, पूर्णाङ्क n);
शून्य translate_array(भिन्न *a, पूर्णाङ्क n, भिन्न s);
भिन्न constrain(भिन्न min, भिन्न max, भिन्न a);
पूर्णाङ्क constrain_int(पूर्णाङ्क a, पूर्णाङ्क min, पूर्णाङ्क max);
भिन्न rand_scale(भिन्न s);
पूर्णाङ्क rand_int(पूर्णाङ्क min, पूर्णाङ्क max);
शून्य mean_arrays(भिन्न **a, पूर्णाङ्क n, पूर्णाङ्क els, भिन्न *avg);
भिन्न dist_array(भिन्न *a, भिन्न *b, पूर्णाङ्क n, पूर्णाङ्क sub);
भिन्न **one_hot_encode(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k);
भिन्न sec(घड़ी_प्रकार clocks);
शून्य print_statistics(भिन्न *a, पूर्णाङ्क n);
पूर्णाङ्क int_index(पूर्णाङ्क *a, पूर्णाङ्क val, पूर्णाङ्क n);

#endif

