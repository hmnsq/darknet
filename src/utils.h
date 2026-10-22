#यदि_अपरिभाषितम् UTILS_H
#परिभाषय UTILS_H
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव "darknet.h"
#अन्तर्भाव "list.h"

#परिभाषय TIME(a) \
    कुरु { \
    द्विगुण start = what_time_is_it_now(); \
    a; \
    प्रारूप_लिख("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } यावत् (0)

#परिभाषय TWO_PI 6.2831853071795864769252866f

द्विगुण what_time_is_it_now();
शून्य shuffle(शून्य *arr, size_t n, size_t आकार);
शून्य sorta_shuffle(शून्य *arr, size_t n, size_t आकार, size_t sections);
शून्य free_ptrs(शून्य **ptrs, पूर्णाङ्क n);
पूर्णाङ्क alphanum_गन्तव्यम्_int(वर्ण c);
वर्ण int_गन्तव्यम्_alphanum(पूर्णाङ्क i);
पूर्णाङ्क read_int(पूर्णाङ्क fd);
शून्य write_int(पूर्णाङ्क fd, पूर्णाङ्क n);
शून्य read_all(पूर्णाङ्क fd, वर्ण *बफर, size_t अष्टकानि);
शून्य write_all(पूर्णाङ्क fd, वर्ण *बफर, size_t अष्टकानि);
पूर्णाङ्क read_all_fail(पूर्णाङ्क fd, वर्ण *बफर, size_t अष्टकानि);
पूर्णाङ्क write_all_fail(पूर्णाङ्क fd, वर्ण *बफर, size_t अष्टकानि);
शून्य find_replace(वर्ण *str, वर्ण *orig, वर्ण *rep, वर्ण *output);
शून्य malloc_error();
शून्य file_error(वर्ण *s);
शून्य strip(वर्ण *s);
शून्य strip_char(वर्ण *s, वर्ण bad);
list *विभज_str(वर्ण *s, वर्ण delim);
वर्ण *fgetl(FILE *fp);
list *parse_csv_पङ्क्तिः(वर्ण *पङ्क्तिः);
वर्ण *copy_string(वर्ण *s);
पूर्णाङ्क गणनम्_fields(वर्ण *पङ्क्तिः);
भिन्न *parse_fields(वर्ण *पङ्क्तिः, पूर्णाङ्क n);
शून्य translate_array(भिन्न *a, पूर्णाङ्क n, भिन्न s);
भिन्न constrain(भिन्न min, भिन्न max, भिन्न a);
पूर्णाङ्क constrain_int(पूर्णाङ्क a, पूर्णाङ्क min, पूर्णाङ्क max);
भिन्न rand_scale(भिन्न s);
पूर्णाङ्क rand_int(पूर्णाङ्क min, पूर्णाङ्क max);
शून्य माध्यम्_arrays(भिन्न **a, पूर्णाङ्क n, पूर्णाङ्क els, भिन्न *avg);
भिन्न dist_array(भिन्न *a, भिन्न *b, पूर्णाङ्क n, पूर्णाङ्क sub);
भिन्न **one_hot_encode(भिन्न *a, पूर्णाङ्क n, पूर्णाङ्क k);
भिन्न sec(clock_t clocks);
शून्य print_statistics(भिन्न *a, पूर्णाङ्क n);
पूर्णाङ्क int_सूचकांक(पूर्णाङ्क *a, पूर्णाङ्क val, पूर्णाङ्क n);

#यदि_अन्तः

