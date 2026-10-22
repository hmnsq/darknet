#अगर_अघोषित UTILS_H
#घोषणा UTILS_H
#समावेश <stdio.h>
#समावेश <time.h>
#समावेश "darknet.h"
#समावेश "list.h"

#घोषणा TIME(a) \
    करो { \
    द्विगुन start = what_time_is_it_now(); \
    a; \
    म_लिखो("%s took: %f seconds\n", #a, what_time_is_it_now() - start); \
    } जबतक (0)

#घोषणा TWO_PI 6.2831853071795864769252866f

द्विगुन what_time_is_it_now();
व्योम shuffle(व्योम *arr, माप_प्रकार n, माप_प्रकार आकार);
व्योम sorta_shuffle(व्योम *arr, माप_प्रकार n, माप_प्रकार आकार, माप_प्रकार sections);
व्योम free_ptrs(व्योम **ptrs, पूर्णांक n);
पूर्णांक alphanum_to_int(अक्षर c);
अक्षर int_to_alphanum(पूर्णांक i);
पूर्णांक read_int(पूर्णांक fd);
व्योम write_int(पूर्णांक fd, पूर्णांक n);
व्योम read_all(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes);
व्योम write_all(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes);
पूर्णांक read_all_fail(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes);
पूर्णांक write_all_fail(पूर्णांक fd, अक्षर *बफर, माप_प्रकार bytes);
व्योम find_replace(अक्षर *str, अक्षर *orig, अक्षर *rep, अक्षर *output);
व्योम malloc_error();
व्योम file_error(अक्षर *s);
व्योम strip(अक्षर *s);
व्योम strip_char(अक्षर *s, अक्षर bad);
list *split_str(अक्षर *s, अक्षर delim);
अक्षर *fgetl(खाता *fp);
list *parse_csv_पंक्ति(अक्षर *पंक्ति);
अक्षर *copy_string(अक्षर *s);
पूर्णांक गिनती_fields(अक्षर *पंक्ति);
भग्न *parse_fields(अक्षर *पंक्ति, पूर्णांक n);
व्योम translate_array(भग्न *a, पूर्णांक n, भग्न s);
भग्न constrain(भग्न min, भग्न max, भग्न a);
पूर्णांक constrain_int(पूर्णांक a, पूर्णांक min, पूर्णांक max);
भग्न rand_scale(भग्न s);
पूर्णांक rand_int(पूर्णांक min, पूर्णांक max);
व्योम mean_arrays(भग्न **a, पूर्णांक n, पूर्णांक els, भग्न *avg);
भग्न dist_array(भग्न *a, भग्न *b, पूर्णांक n, पूर्णांक sub);
भग्न **one_hot_encode(भग्न *a, पूर्णांक n, पूर्णांक k);
भग्न sec(घड़ी_प्रकार clocks);
व्योम print_statistics(भग्न *a, पूर्णांक n);
पूर्णांक int_सूचकांक(पूर्णांक *a, पूर्णांक val, पूर्णांक n);

#पूर्ण_अगर

